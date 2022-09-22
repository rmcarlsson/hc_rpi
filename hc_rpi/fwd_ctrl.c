/*
 * fwd_ctrl.c
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

/*H***************************************************************************
 *
 * $RCSfile: menu.c,v $
 *
 *  Descript: Menu system.
 *
 *   Creator: Mikael Carlsson
 *   Project: wireless lightcontrol
 *
 *   $Author: mika $
 *     $Date: 2003-11-20 20:51:54+01 $
 * $Revision: 1.2 $
 *----------------------------------------------------------------------------
 *      $Log: menu.c,v $
 *      Revision 1.2  2003-11-20 20:51:54+01  mika
 *      Changed menu_t pointer const.
 *
 *      Revision 1.1  2003-11-15 20:07:18+01  mika
 *      Lint and indent. Chaned comments.
 *
 *      Revision 1.0  2003-11-12 20:51:55+01  mika
 *      Initial revision
 *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "fwd_ctrl.h"
#include "esbe3_drv.h"
#include "mylog.h"
#include "temp.h"

#define FWDCTRL_HYSTERESIS ((temperature_t)(2))
#define FWDCTRL_NOF_POINTS ((int)(11))

const temperature_t indoor_target = 18.5;

/*****************************************************************************
 *                                                                            *
 *  P R I V A T E   D A T A                                                   *
 *                                                                            *
 *****************************************************************************/
FwdCtrl_point_t m_points[FWDCTRL_NOF_POINTS] =
    {{FWDCTRL_ID_M30, -30, 40, 18.5},
     {FWDCTRL_ID_M25, -25, 38, 18.5},
     {FWDCTRL_ID_M20, -20, 34, 18.5},
     {FDWCTRL_ID_M15, -15, 34, 18.5},
     {FWDCTRL_ID_M10, -10, 32, 18.5},
     {FWDCTRL_ID_M5, -5, 30, 18.5},
     {FWDCTRL_ID_0, 0, 28, 18.5},
     {FDWCTRL_ID_P5, 5, 26, 18.5},
     {FWDCTRL_ID_P10, 10, 23, 18.5},
     {FWDCTRL_ID_P15, 15, 21, 18.5},
     {FWDCTRL_ID_P20, 20, 20, 18.5}};

static FwdCtrl_mode_t m_mode;
static temperature_t m_target_temp;

/*****************************************************************************
 *                                                                            *
 *  P R I V A T E   M E T H O D S                                             *
 *                                                                            *
 *****************************************************************************/
static void calculate_target(temperature_t outdoor_temp, temperature_t *target_temp, temperature_t indoor_temp);

void FwdCtrl_adjust_target()
{

  for (int pos = 0; pos < FWDCTRL_NOF_POINTS; pos++)
  {
    temperature_t e = indoor_target - m_points[pos].avg_indoor_temp;

    if (abs(e) > 0.5)
    {
      if (e > 0)
        m_points[pos].fwd_temp -= 0.1;
      else
        m_points[pos].fwd_temp += 0.1;
    }
    ml_log(LOG_NOTICE, "Adjusting for target temp at outdoor %-+3.1f: FWD is now %-+3.1f based on indoor avg  %-+3.1f",
           m_points[pos].outdoor_temp, m_points[pos].fwd_temp, m_points[pos].avg_indoor_temp);
  }
}

/*****************************************************************************
 *                                                                            *
 *  P R I V A T E   T Y P E S                                                 *
 *                                                                            *
 *****************************************************************************/

/*****************************************************************************
 *                                                                            *
 *  Global function : Menu_init                                               *
 *                                                                            *
 *  Argument        : none                                                    *
 *                                                                            *
 *  Return          : none                                                    *
 *                                                                            *
 *  Description : Displays the inital string from the menu system.            *
 *                                                                            *
 *                                                                            *
 *****************************************************************************/
void FwdCtrl_init(void)
{
  m_mode = FWDCTRL_MODE_AUTO;
  m_target_temp = m_points[(FWDCTRL_NOF_POINTS - 1)].fwd_temp;
}

void FwdCtrl_set_mode(FwdCtrl_mode_t mode)
{
  m_mode = mode;
}

/*****************************************************************************
 *                                                                            *
 *  Global function : tMenu                                                   *
 *                                                                            *
 *  Argument        : none                                                    *
 *                                                                            *
 *  Return          : none                                                    *
 *                                                                            *
 *  Description : Menu task, handles the menu system at runtime.              *
 *                                                                            *
 *                                                                            *
 *****************************************************************************/
void tFwdCtrl(void)
{
  temperature_t outdoor_temp = 0;
  temperature_t indoor_temp = 0;
  temperature_t fwd_temp = 0;
  temperature_t e = 0;
  temperature_t day_cycle_reduction = 0;
  esbe3_adjust_t esbe_action = ESBE3_NO_ACTION;
  time_t curtime;
  struct tm *loctime = NULL;

  curtime = time(NULL);
  loctime = localtime(&curtime);

  if (m_mode != FWDCTRL_MODE_AUTO)
  {
    return; /* ########## RETURN ########## */
  }
  Temperature_get_temperature(&outdoor_temp, TEMP_SPOT_OUTDOOR);

  Temperature_get_temperature(&indoor_temp, TEMP_SPOT_INDOOR);

  calculate_target(outdoor_temp, &m_target_temp, indoor_temp);

  if ((loctime->tm_hour > 5) && (loctime->tm_hour < 8))
  {
    day_cycle_reduction = (((m_target_temp - m_points[(FWDCTRL_NOF_POINTS - 1)].fwd_temp) * 30) / 100);
  }

  temperature_t target_temp = m_target_temp - day_cycle_reduction;

  Temperature_get_temperature(&fwd_temp, TEMP_SPOT_FWD);
  e = fwd_temp - target_temp;

  ml_log(LOG_NOTICE, "Outdoor is %-+3.1f, FWD is %-+3.1f, target is %-+3.1f (dcr %-+3.1f), e is %-+3.1f", outdoor_temp, fwd_temp, m_target_temp, -day_cycle_reduction, e);

  if (fabs(e) > FWDCTRL_HYSTERESIS)
  {
    if (e < 0)
    {
      esbe_action = ESBE3_STEP_INCREASE;
      ml_log(LOG_NOTICE, "Will increase");
    }
    else
    {
      esbe_action = ESBE3_STEP_DECREASE;
      ml_log(LOG_NOTICE, "Will decreas");
    }
    Esbe3_adjust(esbe_action);
  }
  else
  {
    ml_log(LOG_NOTICE, "Do nothing");
  }
}

static void calculate_target(temperature_t outdoor_temp, temperature_t *target_temp, temperature_t indoor_temp)
{

  int pos = 0;
  temperature_t x1, x2, y1, y2, index;
  temperature_t ret;

  while (m_points[pos].outdoor_temp < outdoor_temp)
  {
    if (pos >= (FWDCTRL_NOF_POINTS - 1))
    {
      *target_temp = m_points[FWDCTRL_NOF_POINTS - 1].fwd_temp;
      return;
    }
    pos++;
  }

  if (pos == 0)
  {
    ret = m_points[0].fwd_temp;
  }
  else
  {
    x2 = m_points[pos].outdoor_temp;
    x1 = m_points[pos - 1].outdoor_temp;

    y2 = m_points[pos].fwd_temp;
    y1 = m_points[pos - 1].fwd_temp;
    index = ((outdoor_temp - x1) * 100) / (x2 - x1);

    ret = (y1 + (index * (y2 - y1) / 100));
  }

  *target_temp = ret;

  m_points[pos].avg_indoor_temp = (m_points[pos].avg_indoor_temp * 80 + indoor_temp) / 81;
}
