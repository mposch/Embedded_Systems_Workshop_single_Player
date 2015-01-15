/*
 * gamelogic.c
 *
 *  Created on: 14 Jan 2015
 *      Author: es
 */
#include "gamelogic.h"
#include "4gew.h"
#define WIN_CONDITION 4
#define rows_of_array(name)       \
    (sizeof(name   ) / sizeof(name[0][0]) / columns_of_array(name))
#define columns_of_array(name)    \
    (sizeof(name[0]) / sizeof(name[0][0]))

int check_win_condition()
{
	int row,col;
	int win=0;
	int oldval=0;
// Cheat
	fields[0][0] = 1;
	fields[0][1] = 1;
	fields[0][2] = 1;
	fields[0][3] = 1;

	// Check the field in rows
	for (row=0;row<GAMEFIELD_WIDTH;row++ )
		for (col=0;col<GAMEFIELD_HEIGTH;col++)
			if ((fields[row][col]) != 0)
			{
				if (oldval == fields[row][col]) win++;
				else win=0;
				if (win>= WIN_CONDITION ) return fields[row][col]; // return which player won
				oldval = fields[row][col];
			}
	return 0;

}



