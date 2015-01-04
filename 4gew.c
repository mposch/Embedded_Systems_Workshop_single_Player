#include <stdio.h>
#include <i2c_keys.h>
#include<4gew.h>
#include <unistd.h>

#include <signal.h>
#include <time.h>

#include <ncurses/ncurses.h>



void show_keystate() {
	// A small test for Keys,..,
	if (i2c_state_change()) {
		printf("Key State all: %x\n", i2c_getKeyState());
		printf("Key State 1 : %i\n", i2c_keypressed(1));
		printf("Key State 2 : %i\n", i2c_keypressed(2));
		printf("Key State 3 : %i\n", i2c_keypressed(3));
		printf("Key State 4 : %i\n", i2c_keypressed(4));
	}
}
int r,c,nrows,ncols;



int main(void) {

	int result = 0;
	timer_t timerid;
//	struct sigevent sev;
//	struct itimerspec its;
//	long long freq_nanosecs;
//	sigset_t mask;
//	struct sigaction sa;
//
//	sa.sa_flags=SA_SIGINFO;  //



	// Create timer

//	timer_create(, &sev, &timerid);

//	printf("timer ID is 0x%lx\n", (long) timerid);
	debug_print("Calling i2c init and test functions..\n",0);
	result = i2c_init();
	if (result != 0)
		debug_print("MAIN: Error I2C initialisation.", 0);

// test ncurses

	  initscr();
	    printw("Hello World");
	    refresh();
	    getch();
	    endwin();
	    return 0;
while (1)
{
	// A small test for Keys,.

	show_keystate();
	usleep(10000);
	if (i2c_keypressed(4)) break;
}

	(void) printf("Terminate\n");
	i2c_close();
	return 0;
}

