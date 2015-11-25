/*
 * log.c
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */


/***************************************************************************
 *            Newfile#2
 *
 *  Fri Nov 23 17:09:18 2007
 *  Copyright  2007  User
 *  Email
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "mylog.h"

#define SIZE 1024



FILE* fd = NULL;


void ml_log(ml_log_prio_t prio, const char *format, ...) {

	va_list arglist;
	char log[SIZE];
	char timestamped[SIZE];
	time_t curtime;
    struct tm* loctime = NULL;

    va_start(arglist, format);
    vsprintf(log, format, arglist);
    va_end(arglist);
    curtime = time(NULL);
	loctime = localtime(&curtime);
    strftime(timestamped, SIZE, "%b %d %H:%M:%S ", loctime);
	strcat(timestamped, log);
	strcat(timestamped, "\n");
	printf("%s", timestamped);
//	fputs((const char*)timestamped, fd);
//		fflush(fd);

}


void ml_open(const char* file) {

	if (fd != NULL) {
		perror("Log already open");
	}

	fd = fopen( file, "a+" );

	if (fd < 0) {
		perror("Unable to open log file");
	}
}


void ml_close() {

	int ret;
	ret = fclose(fd);
	if (ret == EOF) {
 	  perror("Unable to close log file");
	}
}
