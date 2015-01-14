
#ifndef viergew_h

#define viergew_h

extern volatile int player_position[];
extern volatile int active_player;
extern const int column_width;
extern const int line_height;
extern const int column_count;
extern volatile int fields[7][6];

extern int fd;

void print_column(int column);
void clearScreen();
void print_header();
void printLine(int line_number);
void printFields();
void exit_tui();

#endif
