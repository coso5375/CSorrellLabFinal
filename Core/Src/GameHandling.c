/*
 * GameHandling.c
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */
#include "GameHandling.h"

static TetrisBlockPropertiesTypeDef activeBlock;
static uint16_t gameGrid[GRID_HEIGHT / 24][GRID_WIDTH / 24] = {0}; // Game grid (10x13 for 240x320 grid)

static uint32_t firstGameTick;  //starting time; helps find elasped time at end
static uint8_t singles; // game stats / clears
static uint8_t doubles;
static uint8_t tetris;   // # of tetris clears (4 rows)
static uint32_t elapsedTime;


void Display_Start_Screen() // Clears the screen, displays start msg and button
{
    LCD_Clear(0, LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);
    LCD_Draw_Square_Fill(120, 140, 146, LCD_COLOR_BLACK); // black outline
    LCD_Draw_Square_Fill(120, 140, 140, LCD_COLOR_RED); //box in middle of screen
    DisplayBlocks(); //THIS BREAKS THE CODE SOMETIMES???? NO IDEA WHY?? BUFFER ISSUE!
    int x = 58; //message in middle of screen
	int y = 110;
	LCD_DisplayString(x, y, "Press To");
	x=80;
	y=140;
	LCD_DisplayString(x, y, "Start");
}

void Game_Init() // Initializes game stats to 0, clears screen, draws tetris grid & spawns 1st block
{
    firstGameTick = HAL_GetTick(); // used to get elasped time
    singles = 0;
    doubles = 0;
    tetris = 0;
    elapsedTime = 0;
    LCD_Clear(0, LCD_COLOR_BLACK);
    Draw_Tetris_Grid();
    spawnBlock(&activeBlock, GenerateRandomNum());
}

void Draw_Tetris_Grid() // Draws tetris grid lines (for 24x24 cells)
{
	// draw the vertical grid lines
	for (int pixel_x = 0; pixel_x <= GRID_WIDTH; pixel_x += 24) //can eventually take the block as input argument so that block->cellsize can be the size of the grid
	{
		LCD_Draw_Vertical_Line(pixel_x, 0, GRID_HEIGHT-7, LCD_COLOR_WHITE); // -7 bc grid does not take entire height of LCD
	}
	LCD_Draw_Vertical_Line(GRID_WIDTH - 1, 0, GRID_HEIGHT-7, LCD_COLOR_WHITE); // for the right edge of screen

	for (int pixel_y = 0; pixel_y <= GRID_HEIGHT; pixel_y += 24)
	{
		LCD_Draw_Horizontal_Line(0, pixel_y, GRID_WIDTH, LCD_COLOR_WHITE); // draw the horizontl grid lines
	}
}

void Gameplay() // Handles the game tick scheduled event. Moves block down every 3 seconds when theres no collision, checks for full rows
//If collision is met, locks block in place and spawns a new one
{
    if (!detectCollision(&activeBlock, gameGrid, NONE, MOVE_DOWN))
    {
        moveBlock(MOVE_DOWN, &activeBlock);
    }
    else
    {
        lockBlock(&activeBlock, gameGrid);
        int clearedRows = clearTetrisRows(gameGrid);
        if(clearedRows == 1) // updating game stats
        {
        	singles++;
        }
        if(clearedRows == 2)
        {
        	doubles++;
        }
        if(clearedRows == 4)
        {
        	tetris++;
        }
    spawnBlock(&activeBlock, GenerateRandomNum());
    }
}

void RotateEvent()
{
	if (!detectCollision(&activeBlock, gameGrid, ROTATE, ROTATE))
	{
		moveBlock(ROTATE, &activeBlock);
	}

}

void MoveLeftEvent()
{
    if (!detectCollision(&activeBlock, gameGrid, MOVE_LEFT, NONE))
    {
        moveBlock(MOVE_LEFT, &activeBlock);
    }
}

void MoveRightEvent()
{
    if (!detectCollision(&activeBlock, gameGrid, MOVE_RIGHT, NONE))
    {
        moveBlock(MOVE_RIGHT, &activeBlock);
    }
}

void MoveBlockDownEvent() // Moves block all the way to bottom of the grid (DOESNT WORK)
{
	 while (!detectCollision(&activeBlock, gameGrid, NONE, MOVE_DOWN))
	 {
		 moveBlock(MOVE_DOWN, &activeBlock); //move block all the way down (until collision)
	 }
	 //now works but is inconsistent?? check tomorrow
	 lockBlock(&activeBlock, gameGrid); //this handles the same way as tickhandler
	 spawnBlock(&activeBlock, GenerateRandomNum());
}

bool Check_Game_Over() // Goes through the 2nd to top row. If any cell has a block, game is over
{
    for (int col = 0; col < GRID_WIDTH / 24; col++)
    {
        if (gameGrid[2][col] != 0)
        {
            return true;
            //printf("game over");

        }
    }
    return false;
}

void Display_End_Screen() // Display stats, elapsed time
{
    elapsedTime = (HAL_GetTick() - firstGameTick) / 1000; //convert to seconds
    LCD_Clear(0, LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    // "Game Over!"
    int x = 45, y = 10;
    LCD_DisplayString(x, y, "GAME OVER!");

    // "Play Time:"
    x = 10; y = 40;
    LCD_DisplayString(x, y, "Play Time:");

    //print  elapsed time
    int minutes = elapsedTime / 60; //computing minutes & seconds
    int seconds = elapsedTime % 60;
    x = 180;
    LCD_DisplayChar(x, y, (minutes % 10) + '0'); //display the single digit for minutes
    x += 10;
    LCD_DisplayChar(x, y, ':');
    x += 10;
    LCD_DisplayChar(x, y, (seconds / 10) + '0'); //display the tens place seconds
    x += Font16x24.Width;
    LCD_DisplayChar(x, y, (seconds % 10) + '0'); //ones place seconds

    LCD_SetFont(&Font12x12);

    LCD_SetTextColor(LCD_COLOR_RED);
	x = 10; y += 50;
	LCD_DisplayString(x, y, "Single Clears:");
	x = 175;
	LCD_DisplayChar(x, y, (singles / 10) + '0'); // tens
	x += Font12x12.Width;
	LCD_DisplayChar(x, y, (singles % 10) + '0'); //ones

    LCD_SetTextColor(LCD_COLOR_BLUE);
	x = 10; y += 30;
	LCD_DisplayString(x, y, "Double Clears:");
	x = 175;
	LCD_DisplayChar(x, y, (doubles / 10) + '0');
	x += Font12x12.Width;
	LCD_DisplayChar(x, y, (doubles % 10) + '0');

    LCD_SetTextColor(LCD_COLOR_ORANGE);
	x = 10; y += 30;
	LCD_DisplayString(x, y, "Tetris Clears:");
	x = 175;
	LCD_DisplayChar(x, y, (tetris / 10) + '0');
	x += Font12x12.Width;
	LCD_DisplayChar(x, y, (tetris % 10) + '0');

	DisplayBlocks();
}


bool Check_Row_Full(uint16_t gameGrid[GRID_HEIGHT / 24][GRID_WIDTH / 24], int startingRow)
{
    for (int col = 0; col < 10; col++)
    {
        if (gameGrid[startingRow][col] == 0)
        {
            return false;
        }
    }
    return true; //every cell in the row !=0, so row is full
}

void shiftAndClearRows(uint16_t gameGrid[GRID_HEIGHT / 24][GRID_WIDTH / 24], int startingRow)
{
    for (int row = startingRow; row > 0; row--) //start at current row and move UP
    {
        for (int col = 0; col < 10; col++)  //iterate through the row
        {
            gameGrid[row][col] = gameGrid[row - 1][col]; // copy the cells and their color and move them down 1 row
            if (gameGrid[row][col] != 0) 	//if cell has a block there, redraw the color, otherwise draw black
            {		// 12 = block->cellsize / 2
                LCD_Draw_Square_Fill((col*24) + 12, (row*24) + 12, 24, gameGrid[row][col]); //overwrite cells using pixel coordinates (row*24, col*24), using prev block color
            }
            else
            {
                LCD_Draw_Square_Fill((col*24) + 12, (row*24)+ 12, 24, LCD_COLOR_BLACK);
            }
        }
    }
}

int clearTetrisRows(uint16_t gameGrid[GRID_HEIGHT / 24][GRID_WIDTH / 24]) // WORKS NOW!!!!
{
    int lines_cleared = 0;
    for (int row = 12; row >= 0; row--) // start @ bottom row and move up
    {
        if (Check_Row_Full(gameGrid, row)) // check for a full row
        {
            lines_cleared++;
            shiftAndClearRows(gameGrid, row);
            row++; 		//check row again after shift took place
			//printf("row cleared");

        }
    }
    Draw_Tetris_Grid();
    return lines_cleared;
}
