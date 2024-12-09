/*
 * Blocks.c
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */
#include <BlockHandling.h>

const BlockShape T = {
    .shape_rotations = {
        0b0000010011100000, // default rotation
        0b0000010001100100, // 90-degree rotation
        0b0000000011100100, // 180-degree rotation
        0b0000001001100010  //270-degree rotation
    },
	.color = LCD_COLOR_MAGENTA
};

const BlockShape I = {
    .shape_rotations = {
        0b0100010001000100,
        0b0000111100000000,
        0b0100010001000100,
        0b0000111100000000
    },
    .color = LCD_COLOR_CYAN
};

const BlockShape L = {
    .shape_rotations = {
        0b0100010001100000,
        0b0000011101000000,
        0b0110001000100000,
        0b0000001011100000
    },
    .color = LCD_COLOR_ORANGE
};

const BlockShape J = {
    .shape_rotations = {
        0b0010001001100000,
        0b0000010001110000,
        0b0110010001000000,
        0b0000011100010000
    },
    .color = LCD_COLOR_BLUE
};

const BlockShape O = {
    .shape_rotations = {
        0b0000000011001100,
        0b0000000011001100,
        0b0000000011001100,
        0b0000000011001100
    },
    .color = LCD_COLOR_YELLOW
};

const BlockShape S = {
    .shape_rotations = {
        0b0000001101100000,
        0b0000010001100010,
        0b0000011011000000,
        0b0100011000100000  // (MIGHT BE WRONG)
    },
    .color = LCD_COLOR_GREEN
};

const BlockShape Z = {
    .shape_rotations = {
        0b0000110001100000,
        0b0010011001000000,
        0b0000011000110000,
        0b0000100011000100  //(MIGHT ALSO BE WRONG)
    },
    .color = LCD_COLOR_RED
};

void rotateBlock(TetrisBlockPropertiesTypeDef *block)
{
    deleteBlock(block); // delete the previous shape off the screen
    block->rotation = (block->rotation + 1) % 4; // new rotation will be: (0 + 1) % 4 = 1 (modulo so rotation never exceeds 3)
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

void drawBlock(const TetrisBlockPropertiesTypeDef *block)
{
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; // get the 16bit shape for the block depending on rotation
    uint16_t blockColor = block->shapeType->color;

    for (int row = 0; row < 4; row++) //loop through the shapes 4x4 grid
    {
        uint8_t rowBits = getRowBits(shapeData, row); //shift the row (4 bits) to the least significant 4 bits, then mask with 0xF to isolate the 4 bits.
        //rowBits will hold the 4 bits of the current row
        for (int col = 0; col < 4; col++) //iterate through the 4 columns of the current row
        {
            if (isCellInShape(rowBits, col) == true)  // IF cell is apart of our block shape
            {
                int pixel_x = block->x + col * block->cellsize; //get pixel coordinates of the cell
                int pixel_y = block->y + row * block->cellsize;
                LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize, LCD_COLOR_WHITE);
				LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize - 2, blockColor);
            }
        }
    }
}

void deleteBlock(const TetrisBlockPropertiesTypeDef *block)
{
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; //grab the 16 bit shape

    for (int row = 0; row < 4; row++) //loop through the 4x4 grid of the shape
    {
        uint8_t rowBits = getRowBits(shapeData, row);
        for (int col = 0; col < 4; col++)
        {
            if (isCellInShape(rowBits, col) == true)  // IF cell is apart of our block shape
            {
                int pixel_x = block->x + col * block->cellsize; // calculate pixel coordinates
                int pixel_y = block->y + row * block->cellsize;

                LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize, LCD_COLOR_BLACK);
                //draw black square to "erase"
                LCD_Draw_Horizontal_Line(pixel_x, pixel_y, block->cellsize, LCD_COLOR_WHITE); // redraw the white gridlines
                LCD_Draw_Horizontal_Line(pixel_x, pixel_y + block->cellsize, block->cellsize, LCD_COLOR_WHITE);
                LCD_Draw_Vertical_Line(pixel_x, pixel_y, block->cellsize, LCD_COLOR_WHITE);
                LCD_Draw_Vertical_Line(pixel_x + block->cellsize, pixel_y, block->cellsize, LCD_COLOR_WHITE);
            	LCD_Draw_Vertical_Line(GRID_WIDTH - 1, 0, GRID_HEIGHT-7, LCD_COLOR_WHITE); // for the right edge of screen
            }
        }
    }
}

void lockBlock(const TetrisBlockPropertiesTypeDef *block, uint16_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize])
{
	//**need to edit this function so that it checks for collisions before spawning a block (if grid gets full)
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; // get the 16 bits for the shape
    int blockTopLeftX = block->x / block->cellsize; // get the top left (x,y) coords in the grid
    int blockTopLeftY = block->y / block->cellsize;

    for (int row = 0; row < 4; row++) // loop thru the blocks 4x4 grid of bits
    {
        uint8_t rowBits = getRowBits(shapeData, row);
        for (int col = 0; col < 4; col++)
        {
            if (isCellInShape(rowBits, col) == true)  // IF cell is apart of our block shape
            {
                int GridCellX = blockTopLeftX + col; // grab the (x,y) coords of the current cell
                int GridCellY = blockTopLeftY + row;
                // permanently lock the block into the game grid by setting the color of the cells
                gameGrid[GridCellY][GridCellX] = block->shapeType->color;
            }
        }
    }
}

void spawnBlock(TetrisBlockPropertiesTypeDef *block, uint32_t blockType)
{
    const BlockShape* shapeType[] = {&T, &I, &L, &J, &O, &S, &Z}; // array for all 7 blocks so that random num gen can provide an int 1-7
	block->cellsize = 24; // initialize cellsize to 24 (should be smaller for a real 10x20 tetris grid)
    block->x = (GRID_WIDTH / 2) - (block->cellsize); // setting start x coord
    block->y = 0; // start y coord
    block->block_color = shapeType[blockType-1]->color; // initialize block color
    block->rotation = 0; // start at NO rotation
    block->shapeType = shapeType[blockType -1];
    drawBlock(block);
}

bool detectCollision(const TetrisBlockPropertiesTypeDef *block, uint16_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize], int MOVE_X, int MOVE_Y)
{
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; // grab 16-bit shape
    int blockTopLeftX = (block->x / block->cellsize) + MOVE_X; // find the x,y coords of the shape in the grid
    int blockTopLeftY = (block->y / block->cellsize) + MOVE_Y;
    for (int row = 0; row < 4; row++)
    {
        uint8_t rowBits = getRowBits(shapeData, row);

        for (int col = 0; col < 4; col++)
        {
            if (isCellInShape(rowBits, col) == true)  // IF cell is apart of our block shape
            {
                int GridCellX = blockTopLeftX + col; // get the current x,y cell position
                int GridCellY = blockTopLeftY + row;

                if (MOVE_X != NONE && (GridCellX < 0 || GridCellX >= (GRID_WIDTH / block->cellsize)))
                {
                    return true; // collision with edge of grid, cant move left/right
                }
                if (MOVE_Y != NONE && GridCellY >= (GRID_HEIGHT / block->cellsize))
                {
                    return true; // collision with bottom of grid, cant move down
                }
                if (gameGrid[GridCellY][GridCellX] != 0)
                {
                    return true; // we have a collision with an existing block
                }
            }
        }
    }
    return false; //NO collisions
}

bool canRotate(const TetrisBlockPropertiesTypeDef *block, uint16_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize])
{
    //here we will "pretend" the rotation goes through
    int rotation = (block->rotation + 1) % 4; // find the next rotation #
    uint16_t rotatedShapeData = block->shapeType->shape_rotations[rotation]; // get the 16 bit shape data for our newly rotated shape
    int blockTopLeftX = block->x / block->cellsize; //grab cell x,y coords
    int blockTopLeftY = block->y / block->cellsize;

    for (int row = 0; row < 4; row++)  //iterate through the rotated blocks 4x4 block shape
    {
        uint8_t rowBits = getRowBits(rotatedShapeData, row);

        for (int col = 0; col < 4; col++)
        {
            if (isCellInShape(rowBits, col) == true)  // check each bit, if we have a shape there, we need to fill in that dell
            {
                int GridCellX = blockTopLeftX + col; // grabbing block cells x,y coords
                int GridCellY = blockTopLeftY + row;

                //checking allowed boundaries
                if (GridCellX < 0 || GridCellX >= (GRID_WIDTH / block->cellsize))
                {
                    return false; //collision so block cannot rotqte
                }
                if (GridCellY < 0 || GridCellY >= (GRID_HEIGHT / block->cellsize))
                {
                	return false;
                }
                //checking for collisions with other blocks already placed
                if (gameGrid[GridCellY][GridCellX] != 0)
                {
                    return false;
                }
            }
        }
    }
    return true; //no rotation collision detected
}

void drawBlockBlackOutline(const TetrisBlockPropertiesTypeDef *block) // just used for displayBlocks()
{
    uint16_t shapeData = block->shapeType->shape_rotations[block->rotation % 4]; // get the 16bit shape for the block depending on rotation
    uint16_t blockColor = block->shapeType->color; // grab the shapes color

    for (int row = 0; row < 4; row++) //loop through the shapes 4x4 grid
    {
        uint8_t rowBits = getRowBits(shapeData, row);
        //shift the row (4 bits) to the least significant 4 bits, then mask with 0xF to isolate the 4 bits.
        //rowBits will hold the 4 bits of the current row
        for (int col = 0; col < 4; col++) //iterate through the 4 columns of the current row
        {
            if (isCellInShape(rowBits, col) == true)  // check each bit, if we have a shape there, we need to fill in that dell
            {
                int pixel_x = block->x + col * block->cellsize; //get pixel coordinates of the cell
                int pixel_y = block->y + row * block->cellsize;

                LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize, LCD_COLOR_BLACK);
				// draw color slightly smaller to leave the border
				LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize - 2, blockColor);
            }
        }
    }
}

void DisplayBlocks() // aesthetics for start and end screen
{
	TetrisBlockPropertiesTypeDef block;

	block.cellsize = 24;
	block.rotation = 0;
	block.shapeType = &I;
	block.x = 192;
	block.y = 224;
    drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shapeType = &Z;
	block.x = 24;
	block.y = 248;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shapeType = &T;
	block.x = 96;
	block.y = 248;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shapeType = &L;
	block.x = -24;
	block.y = 248;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shapeType = &S;
	block.x = 48;
	block.y = 224;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 1;
	block.shapeType = &J;
	block.x = 0;
	block.y = 200;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shapeType = &O;
	block.x = 168;
	block.y = 224;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 1;
	block.shapeType = &L;
	block.x = 120;
	block.y = 224;
	drawBlockBlackOutline(&block);
}

//helper functions to check if current cell is within the shape of the block
uint8_t getRowBits(uint16_t shapeData, int row)
{
	uint8_t rowBits = (shapeData >> (12 - row * 4)) & 0xF;
	return rowBits;
}

bool isCellInShape(uint8_t rowBits, int col)
{
    uint8_t column_bitmask = 1 << (3 - col);
    if((rowBits & column_bitmask) != 0)
    {
    	return true;
    }
    else
    {
    	return false;
    }
}

//RNG FUNCTIONS
RNG_HandleTypeDef hrng;

void RNG_Init() // pretty sure theres a HAL function that does this for us? Cant find the proper one
{
	__HAL_RCC_RNG_CLK_ENABLE(); //enable RNG clock
	hrng.Instance = RNG; // set instance
	if (HAL_RNG_Init(&hrng) != HAL_OK) //check if hal is OK
	{
		while(1)
		{
			//eror handler
		}
	}
}

uint32_t GenerateRandomNum()
{
    uint32_t randomNumber;
    if (HAL_RNG_GenerateRandomNumber(&hrng, &randomNumber) != HAL_OK) // generate num & check if hal OK
    {
        while(1)
        {
        //Error handler
        }
    }
    return (randomNumber % 7) + 1; // return a random number 1 through 7
}


