/*
 * gamelogic.h
 *
 *  Created on: 14 Jan 2015
 *      Author: es
 */

#ifndef GAMELOGIC_H_
#define GAMELOGIC_H_

int check_win_condition(int start_column,int start_row);
void announce_winner(int winner);
void change_player();
int drop();


#endif /* GAMELOGIC_H_ */
