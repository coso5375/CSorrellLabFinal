/*
 * Game.h
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */

#ifndef INC_GAMEHANDLING_H_
#define INC_GAMEHANDLING_H_

#include "BlockHandling.h"




//game initilization
void Game_Init();

//screen state functions
void Display_Start_Screen();
void Draw_Tetris_Grid();
bool Check_Game_Over();
void Display_End_Screen();

//movement functions
void Gameplay();
void RotateEvent();
void MoveLeftEvent();
void MoveRightEvent();
void MoveBlockDownEvent();

//row clearing functions
int clearTetrisRows(uint16_t gameGrid[GRID_HEIGHT / GRID_CELLSIZE][GRID_WIDTH / GRID_CELLSIZE]);
void shiftAndClearRows(uint16_t gameGrid[GRID_HEIGHT / GRID_CELLSIZE][GRID_WIDTH / GRID_CELLSIZE], int startingRow);
bool Check_Row_Full(uint16_t gameGrid[GRID_HEIGHT / GRID_CELLSIZE][GRID_WIDTH / GRID_CELLSIZE], int startingRow);



#endif /* INC_GAMEHANDLING_H_ */
