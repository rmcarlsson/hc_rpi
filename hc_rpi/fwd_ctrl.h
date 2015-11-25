/*
 * fwd_ctrl.h
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

#ifndef FWD_CTRL_H_
#define FWD_CTRL_H_

/*H***************************************************************************
 *
 * $RCSfile: menu_ah_freq.h,v $
 *
 *  Description: Menu system.
 *
 *      Creator: Mikael Carlsson
 *      Project: Wireless lightcontrol
 *
 *      $Author: mika $
 *        $Date: 2003-11-16 20:21:27+01 $
 *    $Revision: 1.0 $
 *----------------------------------------------------------------------------
 * $Log: menu_ah_freq.h,v $
 * Revision 1.0  2003-11-16 20:21:27+01  mika
 * Initial revision
 *
 *
 ****************************************************************************/
#include "temp.h"


/*****************************************************************************
*                                                                            *
*  E X P O R T E D   C O N S T A N T S                                       *
*                                                                            *
*****************************************************************************/


/*****************************************************************************
*                                                                            *
*  E X P O R T E D   T Y P E S                                               *
*                                                                            *
*****************************************************************************/
typedef enum {
	FWDCTRL_ID_M30,
	FWDCTRL_ID_M25,
	FWDCTRL_ID_M20,
	FDWCTRL_ID_M15,
	FWDCTRL_ID_M10,
	FWDCTRL_ID_M5,
	FWDCTRL_ID_0,
	FDWCTRL_ID_P5,
	FWDCTRL_ID_P10,
	FWDCTRL_ID_P15,
	FWDCTRL_ID_P20
} FwdCtrl_id_t;

typedef enum {
	FDWCTRL_MODE_MANUAL,
	FWDCTRL_MODE_AUTO
} FwdCtrl_mode_t;

typedef enum {
	FDWCTRL_POINT_POS_CURRENT,
	FWDCTRL_POINT_POS_NEXT
} FwdCtrl_point_pos_t;



typedef struct {
	const FwdCtrl_id_t    id;
	const temperature_t   outdoor_temp;
	temperature_t         fwd_temp;
} FwdCtrl_point_t;

/*****************************************************************************
*                                                                            *
*  E X P O R T E D  M E T H O D S                                            *
*                                                                            *
*****************************************************************************/
void tFwdCtrl(void);
void FwdCtrl_set_mode(FwdCtrl_mode_t mode);
void FwdCtrl_init();





#endif /* FWD_CTRL_H_ */
