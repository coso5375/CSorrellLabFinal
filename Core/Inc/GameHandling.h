/*
 * Game.h
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */

#ifndef INC_GAMEHANDLING_H_
#define INC_GAMEHANDLING_H_

#include "BlockHandling.h"

void Game_Init();
void Display_Start_Screen();
void Draw_Tetris_Grid();
bool Check_Game_Over();
void Display_End_Screen();
void Game_Loop();
void Gameplay();
void RotateEvent();
void MoveLeftEvent();
void MoveRightEvent();
void MoveBlockDownEvent();
uint8_t clearTetrisRows(uint16_t gameGrid[GRID_HEIGHT / 24][GRID_WIDTH / 24]);



#endif /* INC_GAMEHANDLING_H_ */
