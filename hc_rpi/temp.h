/*
 * temp.h
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

#ifndef TEMP_H_
#define TEMP_H_

/*H***************************************************************************
 *
 * $Workfile: temp.h $
 *
 *  Descript: Temperature application.
 *
 *   Creator: Mikael Carlsson
 *   Project: Greenhouse control
 *
 *   $Author: emwmika $
 *     $Date: 2003-09-17 11:23:04+02 $
 * $Revision: 1.0 $
 *----------------------------------------------------------------------------
 *      $Log: temp.h,v $
 *      Revision 1.0  2003-09-17 11:23:04+02  emwmika
 *      Initial revision
 *
 *
 * 1     :1-12-30 19:53 Mika
 * First version.
 *
 * 1     :1-12-30 14:45 Mika
 * First version.
 ****************************************************************************/

/*****************************************************************************
*                                                                            *
*  C O N S T A N T S                                                         *
*                                                                            *
*****************************************************************************/


/*****************************************************************************
*                                                                            *
*  E X P O R T E D   T Y P E S                                               *
*                                                                            *
*****************************************************************************/
typedef float temperature_t;

typedef enum {
	TEMP_SPOT_OUTDOOR = 0,
	TEMP_SPOT_FWD,
	TEMP_SPOT_INDOOR
} temp_spot_id_t;


/*****************************************************************************
*                                                                            *
*  E X P O R T E D  M E T H O D S                                            *
*                                                                            *
*****************************************************************************/
void  Temperature_get_temperature( temperature_t* temp, temp_spot_id_t spot );



#endif /* TEMP_H_ */
