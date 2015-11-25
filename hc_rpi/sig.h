/*
 * sig.h
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

/***************************************************************************
 *            sig.h
 *
 *  Fri Nov 23 20:57:32 2007
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

#ifndef _SIG_H
#define _SIG_H

#ifdef __cplusplus
extern "C"
{
#endif

void termination_handler (int sig);

#ifdef __cplusplus
}
#endif

#endif /* _SIG_H */

