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
static uint8_t singleClears;
static uint8_t doubleClears;
static uint8_t tetrisClears;   // # of tetris clears (4 rows)
static uint32_t elapsedTime;

void Game_Init()
{
    firstGameTick = HAL_GetTick(); // used to get elasped time
    singleClears = 0;
    doubleClears = 0;
    tetrisClears = 0;
    elapsedTime = 0;
    LCD_Clear(0, LCD_COLOR_BLACK);
    Draw_Tetris_Grid();
    spawnBlock(&activeBlock, GenerateRandomNum());
}

void Display_Start_Screen()
{
    //clear screen to white and then display "Start" msg
    LCD_Clear(0, LCD_COLOR_WHITE);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);
    LCD_Draw_Square_Fill(120, 140, 144, LCD_COLOR_BLACK); // black outline
    LCD_Draw_Square_Fill(120, 140, 140, LCD_COLOR_RED); //box in middle of screen
    //DisplayBlocks(); //THIS BREAKS THE CODE???? NO IDEA WHY
    //message in middle of screen
    int x = 58;
	int y = 110;
	LCD_DisplayString(x, y, "Press To");
	x=80;
	y=140;
	LCD_DisplayString(x, y, "Start");

}

void Draw_Tetris_Grid()
{
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
}

void Gameplay()
{
    if (!detectCollision(&activeBlock, gameGrid, NONE, DOWN))
    {
        moveBlockDown(&activeBlock);
    }
    else
    {
        lockBlock(&activeBlock, gameGrid);
        uint8_t clearedRows = clearTetrisRows(gameGrid);
        switch (clearedRows) // update game stats
        {
        case 1:
        	singleClears++;
        	break;
        case 2:
        	doubleClears++;
        	break;
        case 4:
        	tetrisClears++;
        	break;
        default:
        	break;
        }
    spawnBlock(&activeBlock, GenerateRandomNum());
    }
}

void RotateEvent()
{
	if (canRotate(&activeBlock, gameGrid))
	{
		rotateBlock(&activeBlock);
	}
	else
	{
		return; // cant rotate
		//maybe put a debug statement here?
	}
}

void MoveLeftEvent()
{
    if (!detectCollision(&activeBlock, gameGrid, LEFT, NONE))
    {
        moveBlockLeft(&activeBlock);
    }
}

void MoveRightEvent()
{
    if (!detectCollision(&activeBlock, gameGrid, RIGHT, NONE))
    {
        moveBlockRight(&activeBlock);
    }
}

void MoveBlockDownEvent()
{
	 while (!detectCollision(&activeBlock, gameGrid, NONE, DOWN))
	 {
		 moveBlockDown(&activeBlock); //move block all the way down (until collision)
	 }

	 lockBlock(&activeBlock, gameGrid); //this handles the same way as tickhandler
	 spawnBlock(&activeBlock, GenerateRandomNum());
}

bool Check_Game_Over()
{
    for (int col = 0; col < GRID_WIDTH / 24; col++) //goes thru each column in 2nd to top row of grid,
    	//if any top row cell has a block, game is over
    {
        if (gameGrid[2][col] != 0)
        {
            return true;
        }
    }
    return false;
}

void Display_End_Screen()
{
    elapsedTime = (HAL_GetTick() - firstGameTick) / 1000; //convert to seconds
    LCD_Clear(0, LCD_COLOR_BLACK);
    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(LCD_COLOR_WHITE);

    // "Game Over!"
    int x = 45, y = 10;
    LCD_DisplayString(x, y, "Game Over!");

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
    LCD_DisplayChar(x, y, (seconds % 10) + '0'); // display the ones place seconds

    LCD_SetFont(&Font12x12);
    LCD_SetTextColor(LCD_COLOR_RED);

	// "Single Clears:"
	x = 10; y += 50;
	LCD_DisplayString(x, y, "Single Clears:");
	x = 175;
	LCD_DisplayChar(x, y, (singleClears / 10) + '0');
	x += Font12x12.Width;
	LCD_DisplayChar(x, y, (singleClears % 10) + '0');

	// "Double Clears:"
    LCD_SetTextColor(LCD_COLOR_BLUE);
	x = 10; y += 30;
	LCD_DisplayString(x, y, "Double Clears:");
	x = 175;
	LCD_DisplayChar(x, y, (doubleClears / 10) + '0');
	x += Font12x12.Width;
	LCD_DisplayChar(x, y, (doubleClears % 10) + '0');

	// "Tetris Clears:"
    LCD_SetTextColor(LCD_COLOR_ORANGE);
	x = 10; y += 30;
	LCD_DisplayString(x, y, "Tetris Clears:");
	x = 175;
	LCD_DisplayChar(x, y, (tetrisClears / 10) + '0');
	x += Font12x12.Width;
	LCD_DisplayChar(x, y, (tetrisClears % 10) + '0');

	DisplayBlocks();
}


uint8_t clearTetrisRows(uint16_t gameGrid[GRID_HEIGHT / 24][GRID_WIDTH / 24])
{
    uint8_t linesCleared = 0;
    for (int row = 12; row >= 0; row--) //starting at bottom row, traverse upwards
    {
        bool fullRow = true;
        for (int col = 0; col < 10; col++) // check if row is completely full
        {
            if (gameGrid[row][col] == 0) //if any cell is empty, row isn't complete
            {
                fullRow = false;
                break;
            }
        }
        if (fullRow == true)
        {
            linesCleared++;
            for (int col = 0; col < 10; col++) // loop thru and set each cell to 0 to clear it
            {
                gameGrid[row][col] = 0;

                int x = col * 24; // get cell coords
                int y = row * 24;
                LCD_Draw_Square_Fill(x + 12, y + 12, 24, LCD_COLOR_BLACK); //set to black
            }
            for (int r = row; r > 0; r--) // move every row down 1 row
            {
                for (int col = 0; col < 10; col++)
                {
                    gameGrid[r][col] = gameGrid[r - 1][col]; //move the row down

                    int x = col * 24; // get coords
                    int y = r * 24;

                    if (gameGrid[r][col] != 0) // if the cell has a block
                    {
                        LCD_Draw_Square_Fill(x + 12, y + 12, 24, gameGrid[r][col]); // redraw with the old block color
                    }
                    else
                    {
                        LCD_Draw_Square_Fill(x + 12, y + 12, 24, LCD_COLOR_BLACK); // set to black to clear
                    }
                }
            }
            for (int col = 0; col < 10; col++) // clear new top row
            {
                gameGrid[0][col] = 0;
                int x = col * 24;
                LCD_Draw_Square_Fill(x + 12, 12, 24, LCD_COLOR_BLACK); // set top row to black
            }
            row++; // recheck row after shift
        }
        Draw_Tetris_Grid(); // redraw grid
    }
    return linesCleared; //# of lines cleared
}



