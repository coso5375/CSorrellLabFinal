/*
 * Game.h
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */

#ifndef INC_GAMEHANDLING_H_
#define INC_GAMEHANDLING_H_

#include "BlockHandling.h"


#define NONE 0 // Macros for moving block
#define LEFT -1
#define UP -1
#define DOWN 1
#define RIGHT 1

void Game_Init();
void Start_Screen();
void Draw_Tetris_Grid();
void Game_Loop();











#endif /* INC_GAMEHANDLING_H_ */
