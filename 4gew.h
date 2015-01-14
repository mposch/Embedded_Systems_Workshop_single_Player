
#ifndef viergew_h
#define viergew_h
/*
 * Global Defines
 */
#define GAMEFIELD_WIDTH 7 // Defines the number of droppable slots
#define GAMEFIELD_HEIGTH 6 // Defines how large the game field should be

#define ANSWER_TO_EVERYTHING 42 // Obvoius

#define  COLUMN_WIDTH  6 // How many spaces shall be used when
#define  ROW_HEIGHT  6  // how many lines shall be used when drawing the field

//#define column_count  7



extern volatile int player_position[];
extern volatile int active_player;

//extern const int column_width;
//extern const int line_height;
//extern const int column_count;
extern volatile int fields[GAMEFIELD_WIDTH][GAMEFIELD_HEIGTH];


extern int fd;

void print_column(int column);
void clearScreen();
void print_header();
void printLine(int line_number);
void printFields();
void exit_tui();

#endif
