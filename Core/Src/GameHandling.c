/*
 * GameHandling.c
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */

#include "GameHandling.h"

void Game_Init()
{
	Start_Screen();
}

void Start_Screen() // Screen will display "Start" and then start upon screen press
{
//	LCD_SetTextColor(LCD_COLOR_BLACK);
//	LCD_SetFont(&Font16x24);
//
//	LCD_DisplayChar(100,140,'S');
//	LCD_DisplayChar(115,140,'t');
//	LCD_DisplayChar(125,140,'a');
//	LCD_DisplayChar(130,140,'r');
//	LCD_DisplayChar(140,140,'t');

	//THIS IS AN ISSUE, CHECKING FOR TOUCH DATA ALWAYS RESULTS IN CODE FAULT.
	//if (returnTouchStateAndLocation(&StaticTouchData) != STMPE811_State_Pressed)
	//{
		//if touch is valid, draw grid
		Draw_Tetris_Grid();
	//}
}

void Draw_Tetris_Grid()
{
	 LCD_Clear(0, LCD_COLOR_BLACK);

	// draw the vertical grid lines
	for (int x = 0; x <= GRID_WIDTH; x += 24) //can eventually take the block as input argument,
		// so that block->cellsize can be the size of the grid
	{
		LCD_Draw_Vertical_Line(x, 0, GRID_HEIGHT-7, LCD_COLOR_WHITE); // need to perfect this so the -7 is not needed
	}
	LCD_Draw_Vertical_Line(GRID_WIDTH - 1, 0, GRID_HEIGHT-7, LCD_COLOR_WHITE); // for the right edge of screen


	// draw the horizontl grid lines
	for (int y = 0; y <= GRID_HEIGHT; y += 24)
	{
		LCD_Draw_Horizontal_Line(0, y, GRID_WIDTH, LCD_COLOR_WHITE);
	}

	Game_Loop();
}



void Game_Loop()
{
	TetrisBlockPropertiesTypeDef testBlock;
	spawnBlock(&testBlock, &T); //spawn a block
	static uint8_t gameGrid[GRID_HEIGHT / 24][GRID_WIDTH / 24] = {0}; // Game grid (10x13 for 240x320 grid)
	//**need find a way to initialize game grid using block->cellsize instead of "24"

    while (1)
    {
        HAL_Delay(500);

        // testing downward movement
        //rotateBlock(&activeBlock);
        //moveBlockRight(&activeBlock);
        //maybe get game tick here to move the block down constantly??
        if (!detectCollision(&testBlock, gameGrid, NONE, DOWN))
        {
            moveBlockDown(&testBlock); // move down if no collision
        }
        else
        {
            //lock block into place
            lockBlock(&testBlock, gameGrid);

            // spawn a new block
            spawnBlock(&testBlock, &I);
            //rotateBlock(&testBlock); // testing rotate and move (seems to work)
            //moveBlockLeft(&testBlock);

        }

        //next step: while (1) loop, will move block down automatically upon game ticket
        //if button pressed, rotate block
        //if right side of screen pressed (x>120), move block right, else move left
        //check if boundaries are working properly, if so, move on to game logic (score, remove rows, etc)
    }
}



