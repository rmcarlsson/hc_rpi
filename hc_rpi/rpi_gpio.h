/*
 * rpi_gpio.h
 *
 *  Created on: 25 nov 2015
 *      Author: rmcar
 */

#ifndef RPI_GPIO_H_
#define RPI_GPIO_H_

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

int
GPIOExport(int pin);
int
GPIOUnexport(int pin);
int
GPIODirection(int pin, int dir);
int
GPIORead(int pin);
int
GPIOWrite(int pin, int value);

#endif /* RPI_GPIO_H_ */
