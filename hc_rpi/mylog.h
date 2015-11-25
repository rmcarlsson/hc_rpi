/*
 * mylog.h
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

#ifndef MYLOG_H_
#define MYLOG_H_


/***************************************************************************
 *            mylog.h
 *
 *  Fri Nov 23 15:46:22 2007
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


#ifdef __cplusplus
extern "C"
{
#endif


typedef enum {

	LOG_NOTICE,
	LOG_WARNING,
	LOG_ERROR
} ml_log_prio_t;

void ml_open(const char* file);
void ml_close(void);
void ml_log(ml_log_prio_t prio, const char *format, ...);


#ifdef __cplusplus
}
#endif



#endif /* MYLOG_H_ */
