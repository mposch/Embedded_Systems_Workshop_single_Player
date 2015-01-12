
#ifndef viergew_h

#define viergew_h

extern volatile int player1_position;
extern volatile int player2_position;
extern volatile int active_player;
extern const int column_width;
extern const int line_height;
extern const int column_count;

extern int fd;

void clearScreen();
void print_header();
void printLine(int line_number);
void printFields();
void exit_tui();

#endif
