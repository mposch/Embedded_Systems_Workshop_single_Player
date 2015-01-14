#include<stdio.h>
#include"4gew.h"
#define ESC 27

void exit_tui(){
	printf("%c[60;0H",ESC);
	printf("%c[37m",ESC);
}

void clearScreen(){
	printf( "%c[2J", ESC );
	printf("%c[H",ESC); // home position cusor

}

void print_dropped(int column,int row,int player){
	int x = (column*column_width);
	int y = (row*line_height)+8;
	int i = 0;
	int j = 0;
	printf("%c[%d;%dH",ESC,y,x);

	for(i=0;i<line_height;i++){
		for(j=0;j<column_width;j++){
			printf("=");
		}
		printf("%c[%d;%dH",ESC,y+i,x);
	}

}

void print_column(int column){
	int i = 6;
	for(i = 6;i>=0;i--){
		if(fields[column][i] != 0){
			print_dropped(column,i,fields[column][i]);
			break;
		}
	}
}

void print_header(){
	printf("%c[8;0H",ESC); // Position cursor at 8/0
	printf("%c[1J",ESC);  // Clear all above cursor
	int x_position = 0;

	if(active_player){
		printf("%c[34m",ESC); // Change color to blue
	}else{
		printf("%c[32m",ESC); // color green
	}

	int i = 0;

	x_position = player_position[active_player]*column_width+1;

	printf("%c[0;%dH",ESC,x_position); // jump to x_position

	for(i = 0; i < column_width;i++){
		printf("=");
	}

	for(i = 1; i < (line_height - 2);i++){
		printf("%c[%d;%dH",ESC,(i+1),x_position);
		printf("|");
		printf("%c[%d;%dH",ESC,(i+1),(x_position+column_width));
		printf("|");
	}

	fflush(stdout);
	printf("%c[37m",ESC); // change color to white
	printf("%c[?25h",ESC); // hide cursor??
}

void printLine(int line_number){
	int column_number = 0;
	if(line_number % 8){
		for(column_number = 0;column_number < 7;column_number++){
				printf("|");
				int j = 0;
				for(j = 0;j < column_width;j++){
					printf(" ");
				}

			}
		printf("|\n");
	}else{
		for(column_number = 0;column_number < 159;column_number++){
			printf("-");
		}
		printf("-\n");
	}



}

void printFields(){
	printf("%c[37m",ESC);
	printf("%c[10;1H",ESC);
	int i = 0;
	for(i = 0; i < 160;i++){
		printf("_");
	}
	printf("\n");
	int j = 0;
	/*for(i = 0; i < 6;i++){
		printf("|          |\n");
	}*/
	/*for(i = 0; i < 12;i++){
			printf("_");
	}*/
	for(i = 1; i < 49;i++){
		printLine(i);
	}

	fflush(stdout);
	//printf("test\n");

}

// /uClinuxdist/vendors/embemmedArtists/
