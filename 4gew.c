/*There might be an error in this program, can you find it?*/
#include <sys/ioctl.h>
#include "4gew.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>/*getpid(),..*/
#include <stdint.h>/*uint32_t,...*/
#include <signal.h>/*signal(),...*/
#include <sys/ioctl.h>/*ioctl(),...*/
#include <asm-generic/ioctls.h>/*ioctls identifiers*/

volatile int player1_position;
volatile int player2_position;
volatile int active_player;
const int column_width = 13;
const int line_height = 6;
const int column_count = 7;

int fd = -1;

struct data {
	uint8_t gpio_values[5];
	uint8_t i2c_values[4];
};

struct data key_data;
struct data old_data;

void sig_handler(int sig){
	//printf("sig_handler called\n");

	read(fd,&key_data,sizeof(struct data));

	if(active_player){
		if((key_data.gpio_values[2] != old_data.gpio_values[2]) && !key_data.gpio_values[2]){
			if(player2_position < column_count){
				player2_position++;
			}else {

			}
		}else if((key_data.gpio_values[4] != old_data.gpio_values[4]) && !key_data.gpio_values[4]){
			if(player2_position > 0){
				player2_position--;
			}else {

			}
		}else if((key_data.gpio_values[0] != old_data.gpio_values[0]) && !key_data.gpio_values[0]){
			active_player = ~active_player;
		}

	}else{
		if(old_data.gpio_values[2] && !key_data.gpio_values[2]){
			if(player1_position < column_count){
				player1_position++;
			}else {

			}
		}else if(old_data.gpio_values[4] && !key_data.gpio_values[4]){
			if(player1_position > 0){
				player1_position--;
			}else{

			}
		}else if((key_data.gpio_values[0] != old_data.gpio_values[0]) && !key_data.gpio_values[0]){
			active_player = ~active_player;
		}
	}
	int i;
	for(i = 0;i< 5;i++){
		old_data.gpio_values[i] = key_data.gpio_values[i];
	}
	for(i = 0; i < 4;i++){
		old_data.i2c_values[i] = key_data.i2c_values[i];
	}
	print_header();
}

int init_4gew(){
	int i;
	for(i = 0; i < 5;i++){
		key_data.gpio_values[i] = 0;
		old_data.gpio_values[i] = 0;
	}
	for(i = 0; i < 4;i++){
		key_data.i2c_values[i] = 0;
		old_data.i2c_values[i] = 0;
	}


	clearScreen();
	print_header();
	printFields();

	fd = open("/dev/lpc2478_4gew",O_RDONLY);
	if(fd < 0){
		printf("failed to open device file\n");
		return -1;
	}
	int ret = ioctl(fd, CTEST_SETPID,getpid());
	if(ret < 0){
		printf("failed to set PID: %d\n",ret);
		return -1;
	}


	signal(SIGUSR1,sig_handler);



//Disable buffering in console
	system ("/bin/stty raw");
//Disable echoing
	system ("/bin/stty -echo");
	return 0;
}

void exit_4gew(){

	if(fd > 0)
		close(fd);
	exit_tui();
	system ("/bin/stty cooked");
	system ("/bin/stty echo");
}


int main (void)
{
	if(init_4gew()){
		exit_4gew();
		return -1;
	}

	while(42){
		if(getchar() == 27){
			break;
		}
		//getchar();
		//break;

	}
	//sleep(10);
	exit_4gew();
	return 0;
}





