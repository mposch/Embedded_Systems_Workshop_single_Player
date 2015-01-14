
#ifndef viergew_h
#define viergew_h
/*
 * Global Defines
 */
#define FIELD_WIDTH 7
#define FIELD_HEIGTH 6
#define ANSWER_TO_EVERYTHING 42

#define  column_width  13
#define  line_height  6
#define column_count  7



extern volatile int player_position[];
extern volatile int active_player;

//extern const int column_width;
//extern const int line_height;
//extern const int column_count;
extern volatile int fields[FIELD_WIDTH][FIELD_HEIGTH];


extern int fd;

void print_column(int column);
void clearScreen();
void print_header();
void printLine(int line_number);
void printFields();
void exit_tui();

#endif
