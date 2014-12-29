/*
 * I2C
 *
 * This File contains the i2c init routines and a test routines for the keys 1-4.
 * Tarfet Board: Lpc 2478
 *
 *
 */
#include <i2c_keys.h>

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

void i2c_test()
{
	printf("i2c test Function:");

}

int i2c_init()
{
	int f = 0;
	int error = 0;
	char buf[10];

	// Assumption of address
	int addr = 0xC0;


	printf("i2c init Fuction:");
	// Open device. I assume 0 is the correct device.
	 f = open("/dev/i2c-2", O_RDWR);
	 if (f < 0) {
		 printf("open error!");
		 goto cleanup;
	 }
	 // set the salve device address
	 error = ioctl (f,I2C_SLAVE,addr);
	 if (error != 0)
			 {
		 	 	 printf("ioctrl error: %i",error);
		 	 	goto cleanup;
			 }
	 // set one led to on
	 buf[0]=0x00;
	 error = write (f,buf,1);

	 if (error != 0)
	 {
		 printf("Write error!");
		 goto cleanup;
	 }

	return 0;
	cleanup:
	 printf (strerror (errno));
	if (f!=0) close(f);
	printf("cleaning up...");
	return error;
}






