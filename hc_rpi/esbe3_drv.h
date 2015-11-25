/*
 * esbe3_drv.h
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

#ifndef ESBE3_DRV_H_
#define ESBE3_DRV_H_

typedef enum {
	ESBE3_NO_ACTION,
	ESBE3_STEP_INCREASE,
	ESBE3_STEP_DECREASE
} esbe3_adjust_t;
void Esbe3_adjust(esbe3_adjust_t action);
void Esbe3_safe_shutdown( void );


#endif /* ESBE3_DRV_H_ */
