/*
 * Blocks.c
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */
#include <BlockHandling.h>

const BlockShape T = { 		// T block
    .shape_rotations = {
        0b0000010011100000, // default rotation
        0b0000010001100100, // 90-degree rotation
        0b0000000011100100, // 180-degree rotation
        0b0000001001100010  // 270-degree rotation
    },
	.color = LCD_COLOR_MAGENTA
};

const BlockShape I = { 		// I block
    .shape_rotations = {
        0b0100010001000100, // default rotation (0 degrees)
        0b0000111100000000, // 90 degrees
        0b0100010001000100, // 180 degrees
        0b0000111100000000  // 270 degrees
    },
    .color = LCD_COLOR_CYAN
};

void drawBlock(const TetrisBlockPropertiesTypeDef *block)
{
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; // get the 16bit shape for the block depending on rotation
    uint16_t blockColor = block->shapeType->color; // grab the shapes color

    for (int row = 0; row < 4; row++) //loop through the shapes 4x4 grid
    {
        uint8_t rowBits = (shapeData >> (12 - row * 4)) & 0xF; //extract 4 bits (one row) from the 16bit shapeData
        //shift the row (4 bits) to the least significant 4 bits, then mask with 0xF to isolate the 4 bits.
        //rowBits will hold the 4 bits of the current row
        for (int col = 0; col < 4; col++) //iterate through the 4 columns of the current row
        {
            if (rowBits & (1 << (3 - col)))  // check each bit, if the AND results in 1, we need to fill in that dell
            {
                int x = block->x + col * block->cellsize; //get (x,y) coordinates of the cell
                int y = block->y + row * block->cellsize;

                LCD_Draw_Square_Fill(x + block->cellsize / 2, y + block->cellsize / 2, block->cellsize, blockColor); //fill in the cell
            }
        }
    }
}

void deleteBlock(const TetrisBlockPropertiesTypeDef *block)
{
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; //grab the 16 bit shape

    for (int row = 0; row < 4; row++) //loop through the 4x4 grid of the shape
    {
        uint8_t rowBits = (shapeData >> (12 - row * 4)) & 0xF; // rowBits = the 4 bits of current row
        for (int col = 0; col < 4; col++)
        {
            if (rowBits & (1 << (3 - col))) //if the cell is drawn,
            {
                int x = block->x + col * block->cellsize; // calculate coordinates
                int y = block->y + row * block->cellsize;

                LCD_Draw_Square_Fill(x + block->cellsize / 2, y + block->cellsize / 2, block->cellsize, LCD_COLOR_BLACK);
                //draw black square to "erase"
                LCD_Draw_Horizontal_Line(x, y, block->cellsize, LCD_COLOR_WHITE); // redraw the white gridlines
                LCD_Draw_Horizontal_Line(x, y + block->cellsize, block->cellsize, LCD_COLOR_WHITE);
                LCD_Draw_Vertical_Line(x, y, block->cellsize, LCD_COLOR_WHITE);
                LCD_Draw_Vertical_Line(x + block->cellsize, y, block->cellsize, LCD_COLOR_WHITE);
            }
        }
    }
}

bool detectCollision(const TetrisBlockPropertiesTypeDef *block, uint8_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize], int MOVE_X, int MOVE_Y)
{
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; //grab 16 bit shape
    int blockTopLeftX = (block->x / block->cellsize) + MOVE_X; //top left (x,y) coords of the block, taking into account the next 'movement'
    int blockTopLeftY = (block->y / block->cellsize) + MOVE_Y;
    //divide by cell size to get grid coordinates
    // Traverse the 4x4 block shape
    //MOVE_X = -1 moves one cell left
    //MOVE_X = 1 moves one cell right
    //MOVE_Y = 1 moves one cell down (towards floor of the game)
    //MOVE_Y = -1 moves one cell up
    for (int row = 0; row < 4; row++)
    {
        uint8_t rowBits = (shapeData >> (12 - row * 4)) & 0xF; //grab the current 4 digits representing the row

        for (int col = 0; col < 4; col++)
        {
            if (rowBits & (1 << (3 - col))) //if our current cell (current column for our curr row) has a shape
            {
                int cellX = blockTopLeftX + col; //get the exact cell positions (top left of shape + current row/col)
                int cellY = blockTopLeftY + row;

                // checking boundaries
                if (cellX < 0 || cellX >= (GRID_WIDTH / block->cellsize) ||cellY < 0 || cellY >= (GRID_HEIGHT / block->cellsize))
                {
                    return true; // collision detected with boundaries of game grid
                }

                // checking for existing blocks
                if (gameGrid[cellY][cellX] != 0) // rows = y, columns = x
                {
                    return true; //collision w/ another block
                }
            }
        }
    }
    return false; //no collision detected
}

void lockBlock(const TetrisBlockPropertiesTypeDef *block, uint8_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize])
{
	//**need to edit this function so that it checks for collisions before spawning a block (if grid gets full)
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; // get the 16 bits for the shape
    int blockTopLeftX = block->x / block->cellsize; // get the top left (x,y) coords in the grid
    int blockTopLeftY = block->y / block->cellsize;

    for (int row = 0; row < 4; row++) // loop thru the blocks 4x4 grid of bits
    {
        uint8_t rowBits = (shapeData >> (12 - row * 4)) & 0xF; // extract the 4 bits for the current row

        for (int col = 0; col < 4; col++)
        {
            if (rowBits & (1 << (3 - col))) //bitmask to check if the specific bit/cell is "ocupied"
            { // if the cell has a block,
                int gridX = blockTopLeftX + col; // grab the (x,y) coords of the current cell
                int gridY = blockTopLeftY + row;
                // permanently lock the block into the game grid by setting the color of the cells
                gameGrid[gridY][gridX] = block->shapeType->color;
            }
        }
    }
}

void spawnBlock(TetrisBlockPropertiesTypeDef *block, const BlockShape *shapeType)
{
	block->cellsize = 24; // initialize cellsize to 24 (should be smaller for a real 10x20 tetris grid)
    block->x = (GRID_WIDTH / 2) - (block->cellsize); // setting start x coord
    block->y = 48; // start y coord
    block->block_color = shapeType->color; // initialize block color
    block->rotation = 0; // start at NO rotation
    block->shapeType = shapeType;
    drawBlock(block);
}

void rotateBlock(TetrisBlockPropertiesTypeDef *block)
{
    deleteBlock(block); // delete the previous shape off the screen
    block->rotation = (block->rotation + 1) % 4; // new rotation will be: (0 + 1) % 4 = 1 (modulo so rotation never exceeds 3)
    //if block is fully rotated 360 degrees, we would have (3 + 1) % 4 = 0 (back to original rotation)
    drawBlock(block);
}

void moveBlockDown(TetrisBlockPropertiesTypeDef *block)
{
    deleteBlock(block);
    block->y += block->cellsize;
    drawBlock(block);
}

void moveBlockLeft(TetrisBlockPropertiesTypeDef *block)
{
    deleteBlock(block);
    block->x -= block->cellsize;
    drawBlock(block);
}

void moveBlockRight(TetrisBlockPropertiesTypeDef *block)
{
    deleteBlock(block);
    block->x += block->cellsize;
    drawBlock(block);
}

