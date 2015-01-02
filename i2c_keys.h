/*
 * i2c_keys.h
 *
 *  Created on: 29 Dec 2014
 *      Author: es
 */

#ifndef I2C_KEYS_H_
#define I2C_KEYS_H_
// This is the I2C adress if the Led Driver where the keys are connected
#define  I2C_ADDR_4GEW 0x60
#define I2C_BUS_4GEW "/dev/i2c-0"

void i2c_test();

int i2c_init();


#endif /* I2C_KEYS_H_ */
