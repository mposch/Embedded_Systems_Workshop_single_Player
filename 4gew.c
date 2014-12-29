/*There might be an error in this program, can you find it?*/
#include <stdio.h>
#include <i2c_keys.h>

int main (void)
{
	int result = 0;

	(void) printf("Calling i2c init and test functions..\n");
	result =  i2c_init();
	i2c_test();
	result =21;

	if (result != 0) printf("Result is not 0! Error!");

	(void) printf("Terminate\n");
	return 0;
}





