
#ifndef viergew_h
#define viergew_h
/*
 * Global Defines
 */

#define GAMEFIELD_WIDTH 7 // Defines the number of droppable slots
#define GAMEFIELD_HEIGTH 6 // Defines how large the game field should be
#define ANSWER_TO_EVERYTHING 42 // Obvoius

#define  COLUMN_WIDTH  3 // How many spaces shall be used when
#define  ROW_HEIGHT  3 // how many lines shall be used when drawing the field

//#define column_count  7

/*
 * Defines used for VT100 Terminal
 */
#define CURSOR_POS(X,Y) "%c[%d;%dH",ESC,X,Y
#define ESC 27
#define CLS "%c[2J",ESC
#define CLEAR_ABOVE_CURSOR "%c[1J",ESC
#define CLEAR_LINE "%c[2K",ESC
#define HIDE_CURSOR "%c[?25h",ESC
#define BLINK_ON "%c[5m",ESC



#define HOME "%c[H",ESC
#define FG_WHITE "%c[37m",ESC
#define FG_BLUE "%c[34m",ESC
#define FG_GREEN "%c[32m",ESC
#define FG_BLACK "%c[30m",ESC

#define BG_BLUE  "%c[44m",ESC
#define BG_GREEN "%c[42m",ESC
#define BG_DEFAULT "%c[49m",ESC
#define BG_BLACK "%c[40m",ESC

#define LEFT 97
#define RIGHT 115
#define DROP 100

/*
 *
 */
extern volatile int player_position[];
extern volatile int active_player;
extern volatile int fields[GAMEFIELD_WIDTH][GAMEFIELD_HEIGTH];


extern int fd;
/*
 * Global Functions
 */

void print_column(int column);
void print_dropped(int column,int row,int player);

void clearScreen();
void print_header();
void printLine(int line_number);
void printFields();
void exit_tui();
void print_status(char* string,...);


#endif
