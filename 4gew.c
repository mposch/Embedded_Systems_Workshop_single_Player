#include <sys/ioctl.h>
#include "4gew.h"
#include "gamelogic.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>/*getpid(),..*/
#include <stdint.h>/*uint32_t,...*/
#include <signal.h>/*signal(),...*/
#include <sys/ioctl.h>/*ioctl(),...*/
#include <asm-generic/ioctls.h>/*ioctls identifiers*/

/*
 * GLOBAL VARIABLES
 */

volatile int player_position[3];
volatile int active_player;
volatile int fields[GAMEFIELD_WIDTH][GAMEFIELD_HEIGTH];
volatile int winner = 0;

int fd = -1;
int sigcount =0;

struct data {
	uint8_t gpio_values[5];
	uint8_t i2c_values[4];
};

struct data key_data;
struct data old_data;

/*
 * Drop
 * Returns 1 if a item was dropped, 0 Else
 * Returns 2 if A Player has won.
 *
 */

// nothing was dropped (row full)
void sigterm_handler(int sig) {
	// Cleanup resources;

	exit_tui();
	exit_4gew();
	exit(0);
}

void sig_handler(int sig) {
	if (sig == SIGUSR1) {

// Received a signal. Keypressed or Joytick pressed.
		//printf("Sighandler.. %d",sigcount++);
		int i;
		//for ( i = 0;i<5;i++) printf("i2c:%d ", key_data.i2c_values[i]);

		printf(NEWLINE);
		if (winner)
			return; //if there is a winner, don't react to joystick or keys

		read(fd, &key_data, sizeof(struct data));
		// Check Joystick Right
		if ((key_data.gpio_values[2] != old_data.gpio_values[2])
				&& !key_data.gpio_values[2]) {
			if (player_position[active_player] < GAMEFIELD_WIDTH - 1) {
				player_position[active_player]++;
			}
		} else if ((key_data.gpio_values[4] != old_data.gpio_values[4])
				&& !key_data.gpio_values[4]) {
			if (player_position[active_player] > 0) {
				player_position[active_player]--;
			}
		} else if ((key_data.gpio_values[0] != old_data.gpio_values[0])
				&& !key_data.gpio_values[0]) {
			drop();
		}

		else if ((key_data.i2c_values[1] == 0)
				) {
			if (player_position[active_player] < GAMEFIELD_WIDTH - 1) {
				player_position[active_player]++;
			}

		} else if ((key_data.i2c_values[0] == 0)
				) {
			if (player_position[active_player] > 0) {
				player_position[active_player]--;
			}
		} else if ((key_data.i2c_values[2] == 0)
				) {
			drop();
		}


		//printf(NEWLINE);

		for (i = 0; i < 5; i++) {
			old_data.gpio_values[i] = key_data.gpio_values[i];
		}
		for (i = 0; i < 4; i++) {
			old_data.i2c_values[i] = key_data.i2c_values[i];
		}
		print_header();

	}
}

int init_4gew() {
	int i, j;
	active_player = 1;
	for (i = 0; i < 5; i++) {
		key_data.gpio_values[i] = 0;
		old_data.gpio_values[i] = 0;
	}
	for (i = 0; i < 4; i++) {
		key_data.i2c_values[i] = 0;
		old_data.i2c_values[i] = 0;
	}
	for (i = 0; i < GAMEFIELD_WIDTH; i++) {
		for (j = 0; j < GAMEFIELD_HEIGTH; j++) {
			fields[i][j] = 0;
		}
	}

	clearScreen();
	print_header();
	printFields();
	signal(SIGUSR1, sig_handler);
		signal(SIGTERM, sigterm_handler);
// Open the Kernel module
	fd = open("/dev/lpc2478_4gew", O_RDONLY);
	if (fd < 0) {
		printf("failed to open device file\n");
		return -1;
	}
// Set our Process ID to receive Signals
	int ret = ioctl(fd, CTEST_SETPID, getpid());
	if (ret < 0) {
		printf("failed to set PID: %d\n", ret);
		return -1;
	}

//signal(SIGINT, sigterm_handler);

//Disable buffering in console
	system("/bin/stty raw");
//Disable echoing
	system("/bin/stty -echo");
	return 0;
}

void exit_4gew() {

	if (fd > 0)
		close(fd);
	exit_tui();
// Enable buffering and echo  again
	system("/bin/stty cooked");
	system("/bin/stty echo");
}

//int keyboard_input(volatile int player_position[3],
//		volatile int active_player)
int keyboard_input() {
	if (winner)
		return 1;
// Keyboard
	char c;
	c = getchar();
	switch (c) {
	case ESC: {
		raise(SIGTERM);
		return 1;
		break;
	}
	case RIGHT: {
		if (player_position[active_player] < GAMEFIELD_WIDTH - 1) {
			player_position[active_player]++;
		}
		break;
	} // end case
	case LEFT: {
		if (player_position[active_player] > 0) {
			player_position[active_player]--;
		}
		break;
	}

	case DROP: {
		drop();
		break;
	}
	default:
		break;
	}
	return 0;
}

int main(void) {

	if (init_4gew()) {
		exit_4gew();
		return -1;
	}

// The answer to everything is 42
	while (ANSWER_TO_EVERYTHING) {
		// Keyboard
		if (keyboard_input(player_position, active_player))
			break;
		print_header();
	} // end while loop

// Call my own sigterm handler
	sigterm_handler(SIGTERM);
	return 0;
}

