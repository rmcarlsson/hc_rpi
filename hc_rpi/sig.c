/*
 * sig.c
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

/***************************************************************************
 *            sig.c
 *
 *  Fri Nov 23 20:49:14 2007
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

#include <signal.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>

#include "mylog.h"
#include "esbe3_drv.h"

void termination_handler (int sig);
void termination_handler (int sig) {

	ml_log(LOG_NOTICE, "Caught a TERM signal, will shutdown");
	Esbe3_safe_shutdown();
	ml_close();

	exit(0);
}


