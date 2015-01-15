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

int check_win_condition(int start_col,int start_row)
{
	int row,col;
	int win=1;
	int oldval=0;
	//print_status("check_win_condition called: %d %d\n",start_col,start_row);
// Cheat
	/*fields[0][0] = 1;
	fields[0][1] = 1;
	fields[0][2] = 1;
	fields[0][3] = 1;*/

	// Check the field in rows
	/*for (row=0;row<GAMEFIELD_WIDTH;row++ )
		for (col=0;col<GAMEFIELD_HEIGTH;col++)
			if ((fields[row][col]) != 0)
			{
				if (oldval == fields[row][col]) win++;
				else win=0;
				if (win>= WIN_CONDITION ) return fields[row][col]; // return which player won
				oldval = fields[row][col];
			}*/

	//check row
	row = start_row;
	for(col = start_col-1;col >= 0;col--){
		if(fields[col][row] == fields[start_col][start_row]){
			win++;
		}else
			break;
	}
	for(col = start_col+1;col < GAMEFIELD_WIDTH;col++){
		if(fields[col][row] == fields[start_col][start_row]){
			win++;
		}else
			break;
	}

	if(win == WIN_CONDITION)
		return fields[start_col][start_row];

	win = 1;	//not winning from row

	//check column
	col = start_col;
	for(row = start_row-1;row >= 0;row--){
		if(fields[col][row] == fields[start_col][start_row]){
			win++;
			//print_status("%d %d %d?",start_row,start_col,fields[start_col][start_row]);
		}else
			break;
	}
	for(row = start_row+1;row < GAMEFIELD_HEIGTH;row++){
		if(fields[col][row] == fields[start_col][start_row]){
			win++;
		}else
			break;
	}
	//print_status("win count: %d",win);
	if(win == WIN_CONDITION)
		return fields[start_col][start_row];

	win = 1;	//not winning from row

	//check negativ diagonal

	for(row = start_row+1, col = start_col+1;col < GAMEFIELD_WIDTH && row < GAMEFIELD_HEIGTH;col++,row++){
		if(fields[col][row] == fields[start_col][start_row]){
			win++;
		}else
			break;
	}
	for(row = start_row-1, col = start_col-1;col >= 0 && row >= 0;col--,row--){
		if(fields[col][row] == fields[start_col][start_row]){
			win++;
		}else
			break;
	}

	if(win == WIN_CONDITION)
		return fields[start_col][start_row];

	//check positiv diagonal
	win = 1;
	for(row = start_row+1, col = start_col-1;col < GAMEFIELD_WIDTH && row >= 0;col--,row++){
		if(fields[col][row] == fields[start_col][start_row]){
			win++;
		}else
			break;
	}
	for(row = start_row-1, col = start_col+1;col >= 0 && row < GAMEFIELD_HEIGTH;col++,row--){
		if(fields[col][row] == fields[start_col][start_row]){
			win++;
		}else
			break;
	}
	print_status("win: %d",win);
	if(win == WIN_CONDITION)
		return fields[start_col][start_row];

	return 0;

}



