/*
 * I2C
 *
 * This File contains the i2c init routines and a test routines for the keys 1-4.
 * Target Board: Lpc 2478
 *
 *
 */
#include <i2c_keys.h>




static int fd;
static int error;
static uint8_t key_state;

void i2c_test() {
	//debug_print("i2c test Function:");
	debug_print("i2c test  function called", 0);

}
int i2c_init() {
	debug_print("...i2c init Fuction called...",1);
	fd = open(I2C_BUS_4GEW, O_RDWR);
	if (fd < 0) {
		debug_print("Error opening I2C device! ", 1);
		goto cleanup;
	}
	error = ioctl(fd, I2C_SLAVE, I2C_ADDR_4GEW);
	if (error != 0) {
		goto cleanup;
	}
	debug_print("Init Function success!",1);
	// read in current state
	key_state=0;
	i2c_state_change();
	return 0;
	cleanup:
	debug_print(strerror (errno), 1);
	debug_print("Cleaning up I2C ...", 1);
	if (fd != 0)
		close(fd);
	return error;
}

int i2c_close() {
	debug_print("Closing I2C Device..", 1);
	if (fd != 0) {
		error = close(fd);
		if (error < 0) {
			debug_print("Error Closing I2C Device!", 1);
			debug_print(strerror (errno), 1);
			return error;
		}
	}
	return error;
}

/*
 * Returns the state of the Keys as inidacted by the i2c driver.
 *
 *
 */
uint8_t  i2c_getkey_state() {
	uint8_t  value = 0;
	if (read(fd,&value,1) != 1)
	{
		// Read Error
		debug_print("I2C: Error reading from Device!",0);
		debug_print(strerror (errno), 1);
	}
return value;
}
/*
 * Checks if there is a state change.
 * Stores the actual state of the Keys, for evaluation with i2c_keypreessed()
 *
 */
int i2c_state_change()
{
	uint8_t new_state = i2c_getkey_state();
	if (key_state != new_state)
	{
		key_state = new_state;
		return 1;
	}
	// No state change...
	else return 0;
}
/*
 * Returns the stored state of the keys.
 */
uint8_t i2c_getKeyState()
{
	return key_state;
}

/*
 * Returns > 0 when the indicated key is pressed
 * Returns 0 if the key is not pressed
 * Should be called immediately after i2c_state_changed!
 */
int  i2c_keypressed(int key)
{

	if ((~key_state & (1 << (key-1))) != 0) return 1;
	else return 0;
}
