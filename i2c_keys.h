/*
 * i2c_keys.h
 *
 *  Created on: 29 Dec 2014
 *      Author: es
 */
/*
 I2C driver return values:
3f  	00111111	No Buttons pressed
3e	00111110	Key 1 Pressed
3d	00111101	Key 2 Pressed
3b	00111011	Key 4 Pressed
37	00110111	Key 4 Pressed
3c	00111100	Key1+2 Pressed
....
*/

#ifndef I2C_KEYS_H_
#define I2C_KEYS_H_
// This is the I2C adress if the Led Driver where the keys are connected
#define  I2C_ADDR_4GEW 0x60
#define I2C_BUS_4GEW "/dev/i2c-0"

#include <4gew.h>

#include <inttypes.h>
#include <stdio.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<lpc246x.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


void i2c_test();

int i2c_init();
int i2c_close();
// returns the actual state from buttons
uint8_t  i2c_getkey_state();

// Use these function for polling:
int i2c_state_change();
// returns the chached button state:
uint8_t i2c_getKeyState();
int i2c_keypressed(int key);




#endif /* I2C_KEYS_H_ */
