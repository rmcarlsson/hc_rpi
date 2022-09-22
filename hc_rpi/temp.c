/*
 * temp.c
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

/*H***************************************************************************
 *
 * $Workfile: fan_app.c $
 *
 *  Descript: Temperature appliaction.
 *
 *   Creator: Mikael Carlsson
 *   Project: Greenhouse control
 *
 *   $Author: emwmika $
 *     $Date: 2003-09-17 11:23:03+02 $
 * $Revision: 1.0 $
 *----------------------------------------------------------------------------
 *      $Log: temp.c,v $
 *      Revision 1.0  2003-09-17 11:23:03+02  emwmika
 *      Initial revision
 *
 *
 * 1     :1-12-30 14:45 Mika
 * First version.
 ****************************************************************************/

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "temp.h"
#include "mylog.h"

#define NUMBER_OF_TEMPSENSORS  ((int)(3))

typedef struct
{
  temp_spot_id_t spot_id;
  const char* path;
} temp_spot_t;

/*****************************************************************************
 *                                                                            *
 *  P R I V A T E   C O N S T A N T S                                         *
 *                                                                            *
 *****************************************************************************/

const temp_spot_t temp_spots[NUMBER_OF_TEMPSENSORS] =
{{ TEMP_SPOT_OUTDOOR,    "28.F67E05010000" },
 { TEMP_SPOT_FWD,        "28.C7FEFB000000" },
 { TEMP_SPOT_INDOOR,     "28.237A05010000" }};
//      {
//	{ TEMP_SPOT_OUTDOOR, "22-0000001eafca" },
//	{ TEMP_SPOT_FWD, "22-0000001eafca" } };

/*****************************************************************************
 *                                                                            *
 *  P R I V A T E   M E T H O D S                                             *
 *                                                                            *
 *****************************************************************************/

/*****************************************************************************
 *                                                                            *
 *  P R I V A T E   D A T A                                                   *
 *                                                                            *
 *****************************************************************************/
//static temperature_t m_temperature[NUMBER_OF_TEMPSENSORS];

void
Temperature_init (void)
{

}


/*****************************************************************************
 *                                                                            *
 *  Global function : Temperature_get_temperature                             *
 *                                                                            *
 *  Argument        : Fanduty, reference                                      *
 *                                                                            *
 *  Return          : none                                                    *
 *                                                                            *
 '*  Description :                                                             *
 *                                                                            *
 *                                                                            *
 *****************************************************************************/

void
Temperature_get_temperature (temperature_t* temp, temp_spot_id_t spot_id)
{

  char devPath[128]; // Path to device
  char buf[256];     // Data from device
  const char path[] = "/mnt/1wire/";
  ssize_t numRead;
#define TEMP_NO_READ (-99)
  float tempC = TEMP_NO_READ;
  int c;

  for (c = 0; c < NUMBER_OF_TEMPSENSORS; c++)
    {
      if (spot_id == temp_spots[c].spot_id)
	{

	  ml_log (LOG_NOTICE, "Trying to read %s", temp_spots[c].path);

	  sprintf (devPath, "%s/%s/temperature", path, temp_spots[c].path);
	  // Read temp continuously
	  // Opening the device's file triggers new reading
	  int fd = open (devPath, O_RDONLY);
	  if (fd == -1)
	    {
	      ml_log (LOG_ERROR, "Error reading %s, errno =%d, rasing SIGTERM");
	      raise (SIGTERM);
	    }
	  while ((numRead = read (fd, buf, 256)) > 0)
	    {
	      tempC = strtof (buf, NULL);
	    }
	  close (fd);
	  ml_log (LOG_NOTICE, "Got %.3f", tempC);

	  break;
	}
    }
  if (tempC == TEMP_NO_READ)
    {
    ml_log (LOG_ERROR, "Error read failed for some reason");
    raise (SIGTERM);
    }
  *temp = tempC;

}


