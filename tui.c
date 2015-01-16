#include<stdio.h>
#include<stdarg.h>
#include"4gew.h"
#include"gamelogic.h"
#include <signal.h>

//#define CLEAR_ABOVE_CURSOR "%c[37m",ESC

/*
 * exit_tui
 * Clean up VT100 Settings
 *
 */
void exit_tui() {
	printf("%c[60;0H", ESC);
	printf(FG_BLACK);
}
/*
 * Clears the VT100 Screen
 */
void clearScreen() {
	printf(BG_DEFAULT);
	printf(CLS); // Clear screen
	printf(HOME); // home position cusor
}

void print_token(int y, int x, int player) {
	int i = 0;
	int j = 0;
	printf(CURSOR_POS(y,x));
	for (i = 0; i < ROW_HEIGHT; i++) {
		for (j = 0; j < COLUMN_WIDTH; j++) {
			if (player == 1)
				printf(BG_GREEN);
			else if (player == 2)
				printf(BG_BLUE);

			printf(" ");
		}
		printf(CURSOR_POS(y+i,x));
	}
	printf(BG_DEFAULT);
}

// Prints a dropped item at the specified row and column. Uses line_ width and line height.
void print_dropped(int column, int row, int player) {

	int x = (column * COLUMN_WIDTH) + (column + 1) + 1;
	int y = (row * ROW_HEIGHT) + ROW_HEIGHT + 1;
	print_token(y, x, player);
}

/*
 * Prints a column, if necessary print a occupied field.
 * Not necessary at the moment
 */
//void print_column(int column){
//	int i = GAMEFIELD_HEIGTH-1;
//	for(i = GAMEFIELD_HEIGTH-1;i>=0;i--){
//		if(fields[column][i] != 0){
//			print_dropped(column,i,fields[column][i]);
//			break;
//		}
//	}
//}
void print_header() {
	printf(CURSOR_POS(ROW_HEIGHT+1,0)); // Position cursor
	printf(CLEAR_ABOVE_CURSOR); // Clear all above cursor
	int x_position = 0;
	x_position = 2 + player_position[active_player] * (COLUMN_WIDTH + 1);
	print_token(0, x_position, active_player);

	if (active_player == 1)
		print_status("Player 1");
	if (active_player == 2)
		print_status("Player 2");

	fflush(stdout);
	printf(FG_BLACK); // change color to white
	printf("%c[?25h", ESC); // hide cursor??
}

/*
 *  Prints a line of the game field. By repeatly calling this function the whole game field can be displayed.
 *
 */

void printLine(int line_number) {
	int column_number = 0;
	if (line_number % ROW_HEIGHT) {
		// Now we need to print a vertical line and field space.
		for (column_number = 0; column_number < GAMEFIELD_WIDTH;
				column_number++) {
			printf("|"); //
			int j = 0;
			for (j = 0; j < COLUMN_WIDTH; j++) {
				printf(" "); // print spaces to fill up the invividual fields
			}
		}
		printf("|\n"); // Print a vertical line at the and + newline

	} else {
		for (column_number = 0;
				column_number < GAMEFIELD_WIDTH * COLUMN_WIDTH + GAMEFIELD_WIDTH;
				column_number++) {
			printf("-"); // Print a horizontal line to seperate rows.
		}
		printf("-\n"); // Finally print a newline at the end.
	}

}

void inline clear_status() {
	printf(CURSOR_POS(GAMEFIELD_WIDTH*(ROW_HEIGHT+1)+1,0));
	printf(CLEAR_LINE);
}

void print_status(char* string, ...) {
	va_list arg;
	clear_status();
	printf(CURSOR_POS(GAMEFIELD_WIDTH*(ROW_HEIGHT+1)+1,0));
	va_start(arg, string);
	vprintf(string, arg);
	va_end(arg);
}

// Prints the game fields
void printFields() {
	printf(FG_MAGENTA);

	//printf(CURSOR_POS(10,1));
	printf(CURSOR_POS(ROW_HEIGHT+1,0)); // Position cursor
	int i = 0;
	for (i = 1; i < GAMEFIELD_HEIGTH * ROW_HEIGHT + 1; i++) {
		printLine(i); // Print gamefield lines
	}

}

int drop()
{
	int i = GAMEFIELD_HEIGTH - 1;
	for (i = GAMEFIELD_HEIGTH - 1; i >= 0; i--) {
		if (fields[player_position[active_player]][i] == 0) {
			fields[player_position[active_player]][i] = active_player; // S tore occupied field
			print_dropped(player_position[active_player], i, active_player); //Print the token
			// Check win
			if (check_win_condition(player_position[active_player], i)) {
				winner = active_player;
				announce_winner(winner);

				raise(SIGTERM); // After one player won, Quit the program

				return 2;

				// change players
			} // nobody has won, continiue game
			change_player();
			return 1;
		}
	}
		return 0;

}

