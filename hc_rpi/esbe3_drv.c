/*
 * esbe3_drv.c
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

#include <signal.h>
#include <unistd.h>

#include "rpi_gpio.h"
#include "esbe3_drv.h"
#include "mylog.h"

#define POUT_INC 24
#define POUT_DEC 25

#define ESBE3_CHANGE_DIR_LAG_TIME (4)
#define ESBE3_STEP_TIME (3)

const int max_move = 75;

static int moves = 0;

typedef enum
{
  ESBE3_IDLE,
  ESBE3_INCREASE_STEP_REQ,
  ESBE3_DECREASE_STEP_REQ,
  ESBE3_INCREASING,
  ESBE3_DECREASING
} esbe3_state_t;

static esbe3_adjust_t m_last_action;

void init_esbe3_drv()
{

  /*
   * Export digital pin for "slow-PWM"
   */
  if ((-1 == GPIOExport(POUT_INC)) || (-1 == GPIOExport(POUT_DEC)))
  {
    ml_log(LOG_ERROR, "Failed to export GPIO pins");
    raise(SIGTERM);
  }
  /*
   * Set GPIO directions
   */
  if ((-1 == GPIODirection(POUT_INC, OUT)) || (-1 == GPIODirection(POUT_DEC, OUT)))
  {
    ml_log(LOG_ERROR, "Failed to set GPIO pins as output");
    raise(SIGTERM);
  }

  if ((-1 == GPIOWrite(POUT_INC, 0)) || (-1 == GPIOWrite(POUT_INC, 0)))
  {
    ml_log(LOG_ERROR, "Error reading %s, errno =%d, rasing SIGTERM");
    raise(SIGTERM);
  }
}

void Esbe3_adjust(esbe3_adjust_t action)
{

  /* Only allowed to adjust in IDLE state */
  switch (action)
  {
  case ESBE3_STEP_INCREASE:
    if (moves < max_move)
    {
      ml_log(LOG_NOTICE, "Increasing shunt ... (%d)", moves);
      moves++;
      if (-1 == GPIOWrite(POUT_INC, 1))
      {
        ml_log(LOG_ERROR, "Failed to set pin %d to 1", POUT_INC);
        raise(SIGTERM);
      }
    }
    break;

  case ESBE3_STEP_DECREASE:
    if (moves > -max_move)
    {
      ml_log(LOG_NOTICE, "Decreasing shunt ... (%d)", moves);
      moves--;
      if (-1 == GPIOWrite(POUT_DEC, 1))
      {
        ml_log(LOG_ERROR, "Failed to set pin %d to 1", POUT_DEC);
        raise(SIGTERM);
      }
    }
    break;

  default:
    /* panic(); */
    break;
  }

  if (action != m_last_action)
  {
    sleep(ESBE3_CHANGE_DIR_LAG_TIME);
  }
  sleep(ESBE3_STEP_TIME);

  if ((-1 == GPIOWrite(POUT_INC, 0)) || (-1 == GPIOWrite(POUT_DEC, 0)))
  {
    ml_log(LOG_ERROR, "Failed to set pin both PGIO pins to 0 after increas/decrease");
    raise(SIGTERM);
  }

  //ml_log(LOG_NOTICE, "... done.");

  m_last_action = action;
}

void Esbe3_safe_shutdown(void)
{

  if ((-1 == GPIOWrite(POUT_INC, 0)) || (-1 == GPIOWrite(POUT_DEC, 0)))
  {
    ml_log(LOG_ERROR, "Failed to set both pins to 0");
    raise(SIGTERM);
  }

  if ((-1 == GPIODirection(POUT_INC, IN)) || (-1 == GPIODirection(POUT_DEC, IN)))
  {
    ml_log(LOG_ERROR, "Failed to set both GPIO pins to IN direction");
    raise(SIGTERM);
  }

  if ((-1 == GPIOUnexport(POUT_INC)) || (-1 == GPIOUnexport(POUT_DEC)))
  {
    ml_log(LOG_ERROR, "Failed to unexport both GPIO pins");
    raise(SIGTERM);
  }

  ml_log(LOG_NOTICE, "Shunt has saftley ended. Bye");
}
