#include <stdio.h>
#include <i2c_keys.h>
#include<4gew.h>
#include <unistd.h>

int main(void) {
	int result = 0;
	debug_print("Calling i2c init and test functions..\n",0);

	result = i2c_init();
	if (result != 0)
		debug_print("MAIN: Error I2C initialisation.", 0);

	// A small test for Keys,..,

	int i;
	for (i = 0; i < 10000; i++) {
		if (i2c_state_change()) {
			printf("Key State all: %x\n", i2c_getKeyState());
			printf("Key State 1 : %i\n", i2c_keypressed(1));
			printf("Key State 2 : %i\n", i2c_keypressed(2));
			printf("Key State 3 : %i\n", i2c_keypressed(3));
			printf("Key State 4 : %i\n", i2c_keypressed(4));
		}

		usleep(10000);
	}

	(void) printf("Terminate\n");
	i2c_close();
	return 0;
}

