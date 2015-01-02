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
	char buf[100];

	// Assumption of address



	printf("i2c init Fuction:");
	// Open device. I assume 0 is the correct device.
	 f = open(I2C_BUS_4GEW, O_RDWR);
	 if (f < 0) {
		 printf("open error!");
		 goto cleanup;
	 }
	 // set the salve device address
	 int b =0;

	 for ( b = 0;b<3600;b++)
	 {
	//	 sleep(1);



	 error = ioctl (f,I2C_SLAVE,I2C_ADDR_4GEW);
	 if (error != 0)
			 {
		 	 	// printf("ioctrl error: %i at adress %x \n",error,addr);
		 	 	 //goto cleanup;
			 }else
				 {
			//	 printf("IOCTL success at adress %x   ...",addr);
			//	 printf("Probing device.....");
				 	 if ( read (f,buf,10) != 10)
				 	 {
					 // ERROR
				//	 printf ("FAILED\n");
				 	 }else
				 	 {
				 		 printf("\nSUCCESS at adress 0x%x",I2C_ADDR_4GEW);
				 		 printf ("...Raw data: " );
				 		 int i = 0;
				 		 for (i=0;i<10;i++) printf ("%x ",buf[i]);
				 		 // Write back data
				 	//	if (addr==0x60)  write (f,buf,1);

				 	 }


				 }


	 }// end for a

	 // set one led to on


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






