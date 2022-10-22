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
#include <stdbool.h>

#include "temp.h"
#include "mylog.h"

#define NUMBER_OF_TEMPSENSORS ((int)(3))

typedef struct
{
	temp_spot_id_t spot_id;
	const char *path;
} temp_spot_t;
#define N_FILTER_POINTS (20)
typedef struct filter_t
{
	temperature_t vals[N_FILTER_POINTS];
	int index;
} filter_t;

static temperature_t get_filtered_val(filter_t *f, temperature_t new_val);
/*****************************************************************************
 *                                                                            *
 *  P R I V A T E   C O N S T A N T S                                         *
 *                                                                            *
 *****************************************************************************/
#define TEMP_NO_READ (-99.0f)
const temp_spot_t temp_spots[NUMBER_OF_TEMPSENSORS] =
	{{TEMP_SPOT_OUTDOOR, "28.F67E05010000"},
	 {TEMP_SPOT_FWD, "28.C7FEFB000000"},
	 {TEMP_SPOT_INDOOR, "28.237A05010000"}};
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
// static temperature_t m_temperature[NUMBER_OF_TEMPSENSORS];

static filter_t filters[NUMBER_OF_TEMPSENSORS];

void Temperature_init(void)
{
	for (int i = 0; i < NUMBER_OF_TEMPSENSORS; i++)
	{
		filters[i].index = 0;
		for (int u = 0; u < N_FILTER_POINTS; u++)
		{
			filters[i].vals[u] = TEMP_NO_READ;
		}
	}
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

void Temperature_get_temperature(temperature_t *temp, temp_spot_id_t spot_id)
{

	char devPath[128]; // Path to device
	char buf[256];	   // Data from device
	const char path[] = "/mnt/1wire/";
	ssize_t numRead;

	float tempC = TEMP_NO_READ;
	int c;
	bool bad_val = false;

	for (c = 0; c < NUMBER_OF_TEMPSENSORS; c++)
	{
		if (spot_id == temp_spots[c].spot_id)
		{

			// ml_log (LOG_NOTICE, "Trying to read %s", temp_spots[c].path);

			sprintf(devPath, "%s/%s/temperature", path, temp_spots[c].path);
			// Read temp continuously
			// Opening the device's file triggers new reading
			int fd = open(devPath, O_RDONLY);
			if (fd == -1)
			{
				ml_log(LOG_ERROR, "Error reading %s, errno =%d, rasing SIGTERM");
				raise(SIGTERM);
			}
			while ((numRead = read(fd, buf, 256)) > 0)
			{
				tempC = strtof(buf, NULL);
			}
			if (tempC > 60.0f || tempC < -30.0f)
			{
				bad_val = true;
			}
			close(fd);
			ml_log(LOG_NOTICE, "Got %.3f", tempC);

			break;
		}
	}
	if (bad_val || tempC == TEMP_NO_READ)
	{
		ml_log(LOG_ERROR, "Error read bad value %f, using old value %f", tempC, filters[c].vals[filters[c].index]);
		tempC = filters[c].vals[filters[c].index];
	}

	temperature_t t = get_filtered_val(&filters[c], tempC);
	*temp = t;
}
static temperature_t get_filtered_val(filter_t *f, temperature_t new_val)
{
	temperature_t sum = 0;
	int index = f->index;
	index++;
	if (index > N_FILTER_POINTS)
	{
		index = 0;
	}
	f->vals[index] = new_val;
	for (int u = 0; u < N_FILTER_POINTS; u++)
	{
		if (f->vals[u] == TEMP_NO_READ)
			f->vals[u] = new_val;
			
		sum += f->vals[u];
	}
	return (sum / N_FILTER_POINTS);
}