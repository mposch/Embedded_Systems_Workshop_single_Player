#include<stdio.h>
#include"4gew.h"
/*
 * Defines used for VT100 Terminal
 */
#define ESC 27
#define CLS "%c[2J",ESC
#define CLEAR_ABOVE_CURSOR "%c[1J",ESC

#define HOME "%c[H",ESC
#define FG_WHITE "%c[37m",ESC
#define FG_BLUE "%c[34m",ESC
#define FG_GREEN "%c[32m",ESC

#define CURSOR_POS(X,Y) "%c[%d;%dH",ESC,X,Y

//#define CLEAR_ABOVE_CURSOR "%c[37m",ESC


/*
 * exit_tui
 * Clean up VT100 Settings
 *
 */
void exit_tui(){
	printf("%c[60;0H",ESC);
	printf(FG_WHITE);
}
/*
 * Clears the VT100 Screen
 */
void clearScreen(){
	printf(CLS); // Clear screen
	printf(HOME); // home position cusor
}

// Prints a dropped item at the specified row and column. Uses line_ width and line height.
void print_dropped(int column,int row,int player){
	int x = (column*GAMEFIELD_WIDTH);
	int y = (row*GAMEFIELD_HEIGTH)+8;
	int i = 0;
	int j = 0;
	printf(CURSOR_POS(y,x));
	//printf("%c[%d;%dH",ESC,y,x);

	for(i=0;i<GAMEFIELD_HEIGTH;i++){
		for(j=0;j<GAMEFIELD_WIDTH;j++){
			printf("=");
		}
		printf(CURSOR_POS(y+i,x));
		//printf("%c[%d;%dH",ESC,y+i,x);
	}

}
/*
 * Prints a column, if necessary print a occupied field
 */

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

	printf(CURSOR_POS(8,0)); // Position cursor at 8/0
	printf(CLEAR_ABOVE_CURSOR);  // Clear all above cursor

	int x_position = 0;

	if(active_player){
		printf(FG_BLUE); // Change color to blue
	}else{
		printf(FG_GREEN); // color green
	}

	int i = 0;

	x_position = player_position[active_player]*GAMEFIELD_WIDTH+1;

	printf("%c[0;%dH",ESC,x_position); // jump to x_position

	for(i = 0; i < GAMEFIELD_WIDTH;i++){
		printf("=");
	}

	for(i = 1; i < (GAMEFIELD_HEIGTH - 2);i++){
		printf(CURSOR_POS( i+1,x_position) );
		printf("|");
		printf(CURSOR_POS ((i+1),(x_position+GAMEFIELD_WIDTH)));
		printf("|");
	}

	fflush(stdout);
	printf("%c[37m",ESC); // change color to white
	printf("%c[?25h",ESC); // hide cursor??
}

/*
 *  Prints a line of the game field. By repeatly calling this function the whole game field can be displayed.
 *
 */

void printLine(int line_number){
	int column_number = 0;
	if(line_number % ROW_HEIGHT)
	{
		// Now we need to print a vertical line and field space.
		for(column_number = 0;column_number < GAMEFIELD_WIDTH;column_number++){
				printf("|"); //
				int j = 0;
				for(j = 0;j < COLUMN_WIDTH;j++){
					printf(" "); // print spaces to fill up the invividual fields
				}
			}
		printf("|\n"); // Print a vertical line at the and + newline

	}else{
		for(column_number = 0;column_number < GAMEFIELD_WIDTH*COLUMN_WIDTH+GAMEFIELD_WIDTH;column_number++){
			printf("-"); // Print a horizontal line to seperate rows.
		}
		printf("-\n"); // Finally print a newline at the end.
	}



}

// Prints the game fields
void printFields(){
	printf(FG_WHITE);
	printf(CURSOR_POS(10,1));
	int i = 0;
	// Print upper border

	for(i = 0; i < GAMEFIELD_WIDTH*COLUMN_WIDTH+GAMEFIELD_WIDTH+1;i++){
		printf("_"); // Print upper border
	}
	printf("\n");
	int j = 0;
	/*for(i = 0; i < 6;i++){
		printf("|          |\n");
	}*/
	/*for(i = 0; i < 12;i++){
			printf("_");
	}*/
	for(i = 1; i < GAMEFIELD_HEIGTH*ROW_HEIGHT+1;i++){
		printLine(i); // Print gamefield lines
	}

	fflush(stdout);
	//printf("test\n");

}

