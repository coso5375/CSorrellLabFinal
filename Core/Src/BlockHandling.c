/*
 * Blocks.c
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */
#include <BlockHandling.h>

const BlockShape T = { // constant instances of blockshape struct
    .shape_rotations = {
        0b0000010011100000, // no rotation
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

void moveBlock(int MOVE, TetrisBlockPropertiesTypeDef *block)
{
	if(MOVE==MOVE_LEFT)
	{
		deleteBlock(block);
		block->x -= block->cellsize;
		drawBlock(block);
	}
	if(MOVE==MOVE_RIGHT)
	{
		deleteBlock(block);
		block->x += block->cellsize;
		drawBlock(block);
	}
	if(MOVE==MOVE_DOWN)
	{
		deleteBlock(block);
		block->y += block->cellsize;
		drawBlock(block);
	}
	if(MOVE==ROTATE)
	{
		deleteBlock(block); // delete the previous shape off the screen
		block->rotation = (block->rotation + 1) % 4; // new rotation will be: (0 + 1) % 4 = 1 (modulo so rotation never exceeds 3)
		drawBlock(block);
	}
}




void drawBlock(const TetrisBlockPropertiesTypeDef *block)
{
    uint16_t shapeData = block->shape->shape_rotations[block->rotation % 4]; // get the 16bit shape for the block depending on rotation
    uint16_t blockColor = block->shape->color;

    for (int row = 0; row < 4; row++) //loop through the shapes 4x4 grid
    {
        uint8_t rowBits = getRowBits(shapeData, row); //shift the row (4 bits) to the least significant 4 bits, then mask with 0xF to isolate the 4 bits.
        //rowBits will hold the 4 bits of the current row
        for (int col = 0; col < 4; col++) //iterate through the 4 columns of the current row
        {
            if (isCellInShape(rowBits, col) == true)  // IF cell is apart of our block shape
            {
            	drawBlockHelper(block, row, col, blockColor, LCD_COLOR_WHITE);
            }
        }
    }
}

void deleteBlock(const TetrisBlockPropertiesTypeDef *block)
{
    uint16_t shapeData = block->shape->shape_rotations[block->rotation % 4]; //grab the 16 bit shape
    for (int row = 0; row < 4; row++) //loop through the 4x4 grid of the shape
    {
        uint8_t rowBits = getRowBits(shapeData, row);
        for (int col = 0; col < 4; col++)
        {
            if (isCellInShape(rowBits, col) == true)  // IF cell is apart of our block shape
            {
            	drawBlockHelper(block, row, col, LCD_COLOR_BLACK, LCD_COLOR_WHITE);
            	LCD_Draw_Vertical_Line(GRID_WIDTH - 1, 0, GRID_HEIGHT-7, LCD_COLOR_WHITE); // for the right edge of screen
            }
        }
    }
}

void lockBlock(const TetrisBlockPropertiesTypeDef *block, uint16_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize])
{
	//**need to edit this function so that it checks for collisions before spawning a block (if grid gets full)
    uint16_t shapeData = block->shape->shape_rotations[block->rotation % 4]; // get the 16 bits for the shape
    int blockTopLeftX = block->x / block->cellsize; // get the top left (x,y) coords in the grid
    int blockTopLeftY = block->y / block->cellsize;
    for (int row = 0; row < 4; row++) // loop thru the blocks 4x4 grid of bits
    {
        uint8_t rowBits = getRowBits(shapeData, row);
        for (int col = 0; col < 4; col++)
        {
            if (isCellInShape(rowBits, col) == true)  // IF cell is apart of our block shape
            {
                // permanently lock the block into the game grid by setting the color of the cells (x,y coords)
                gameGrid[blockTopLeftY + row][blockTopLeftX + col] = block->shapeType->color;
            }
        }
    }
}

void spawnBlock(TetrisBlockPropertiesTypeDef *block, uint32_t blockType)
{
    const BlockShape* shape[] = {&T, &I, &L, &J, &O, &S, &Z}; // array for all 7 blocks so that random num gen can provide an int 1-7
	block->cellsize = 24; // initialize cellsize to 24 (should be smaller for a real 10x20 tetris grid)
    block->x = (GRID_WIDTH / 2) - (block->cellsize); // setting start x coord
    block->y = 0; // start y coord
    block->block_color = shape[blockType-1]->color; // initialize block color
    block->rotation = 0; // start at NO rotation
    block->shapeType = shape[blockType -1];
    drawBlock(block);
}

bool detectCollision(const TetrisBlockPropertiesTypeDef *block, uint16_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize], int MOVE_X, int MOVE_Y)
{
	int horizontal;
	int vertical;


	if(MOVE_X == MOVE_LEFT)
	{
		horizontal = -1;
	}
	if(MOVE_X == NONE)
	{
		horizontal = 0;
	}
	if(MOVE_X == MOVE_RIGHT)
	{
		horizontal = 1;
	}
	if(MOVE_Y == MOVE_DOWN)
	{
		vertical = 1;
	}
	if(MOVE_Y == NONE)
	{
		vertical = 0;
	}
	if(MOVE_X == ROTATE && MOVE_Y == ROTATE) // CHECKING rotation collisions
	{
		//here we will "pretend" the rotation goes through
		int rotation = (block->rotation + 1) % 4; // find the next rotation #
		uint16_t rotatedShapeData = block->shape->shape_rotations[rotation]; // get the 16 bit shape data for our newly rotated shape
		int blockTopLeftX = block->x / 24; //grab cell x,y coords
		int blockTopLeftY = block->y / 24; // block->cellsize can be used instead of 24

		for (int row = 0; row < 4; row++)  //iterate through the rotated blocks 4x4 block shape
		{
			uint8_t rowBits = getRowBits(rotatedShapeData, row);
			for (int col = 0; col < 4; col++)
			{
				if (isCellInShape(rowBits, col) == true)  // check each bit, if we have a shape there, we need to fill in that dell
				{
					//checking allowed boundaries
					if (blockTopLeftX + col < 0 || blockTopLeftX + col >= (GRID_WIDTH / 24)) // make sure x,y coords inside boundaries
					{
						return true; //collision so block cannot rotqte
					}
					if (blockTopLeftY + row < 0 || blockTopLeftY + row >= (GRID_HEIGHT / 24))
					{
						return true;
					}
					//checking for collisions with other blocks already placed
					if (gameGrid[blockTopLeftY + row][blockTopLeftX + col] != 0)
					{
						return true;
					}
				}
			}
		}
		return false; //no rotation collision detected
	}
	//checking movement collisions
    uint16_t shapeData = block->shape->shape_rotations[block->rotation % 4]; // grab 16-bit shape
    int blockTopLeftX = (block->x / 24) + horizontal; // find the x,y coords of the "new" shape in the grid
    int blockTopLeftY = (block->y / 24) + vertical;
    for (int row = 0; row < 4; row++)
    {
        uint8_t rowBits = getRowBits(shapeData, row);
        for (int col = 0; col < 4; col++)
        {
            if (isCellInShape(rowBits, col) == true)  // IF cell is apart of our block shape
            {
                if (MOVE_X != NONE && (blockTopLeftX + col < 0 || blockTopLeftX + col >= (GRID_WIDTH / 24)))
                {
                    return true; // collision with edge of grid, cant move left/right
                }
                if (MOVE_Y != NONE && blockTopLeftY + row >= (GRID_HEIGHT / 24))
                {
                    return true; // collision with bottom of grid, cant move down
                }
                if (gameGrid[blockTopLeftY + row][blockTopLeftX + col] != 0)
                {
                    return true; // we have a collision with an existing block
                }
            }
        }
    }
    return false; //NO collisions
}

void drawBlockBlackOutline(const TetrisBlockPropertiesTypeDef *block) // just used for displayBlocks()
{
    uint16_t shapeData = block->shape->shape_rotations[block->rotation % 4]; // get the 16bit shape for the block depending on rotation
    uint16_t blockColor = block->shape->color; // grab the shapes color
    for (int row = 0; row < 4; row++) //loop through the shapes 4x4 grid
    {
        uint8_t rowBits = getRowBits(shapeData, row);
        //shift the row (4 bits) to the least significant 4 bits, then mask with 0xF to isolate the 4 bits.
        //rowBits will hold the 4 bits of the current row
        for (int col = 0; col < 4; col++) //iterate through the 4 columns of the current row
        {
            if (isCellInShape(rowBits, col) == true)  // check each bit, if we have a shape there, we need to fill in that dell
            {
            	drawBlockHelper(block, row, col, blockColor, LCD_COLOR_BLACK);
//                int pixel_y = block->y + row * block->cellsize;
//
//                LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize, LCD_COLOR_BLACK);
//				// draw color slightly smaller to leave the border
//				LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize - 2, blockColor);
            }
        }
    }
}

void DisplayBlocks() // aesthetics for start and end screen (displays all 7 blocks)
{
	TetrisBlockPropertiesTypeDef block;

	block.cellsize = 24;
	block.rotation = 0;
	block.shape = &I;
	block.x = 192;
	block.y = 224;
    drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shape = &Z;
	block.x = 24;
	block.y = 248;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shape = &T;
	block.x = 96;
	block.y = 248;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shape = &L;
	block.x = -24;
	block.y = 248;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shape = &S;
	block.x = 48;
	block.y = 224;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 1;
	block.shape = &J;
	block.x = 0;
	block.y = 200;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 0;
	block.shape = &O;
	block.x = 168;
	block.y = 224;
	drawBlockBlackOutline(&block);
	block.cellsize = 24;
	block.rotation = 1;
	block.shape = &L;
	block.x = 120;
	block.y = 224;
	drawBlockBlackOutline(&block);
}

//helper functions to check if current cell is within the shape of the block
uint8_t getRowBits(uint16_t shapeData, int row)
{	//shift the row (4 bits) to the least significant 4 bits, then mask with 0xF to isolate the 4 bits.
	uint8_t rowBits = (shapeData >> (12 - row * 4)) & 0xF;
	return rowBits;	//rowBits will hold the 4 bits of the current row
}

bool isCellInShape(uint8_t rowBits, int col) //check if cell is within the blockshape
{
    uint8_t column = 1 << (3 - col); //shift 1 to column position (will be somthing like 0b0010 depending on col)
    if((rowBits & column) != 0) // if rowBits also has a nonzero there, cell is within the shape
    {
    	return true;
    }
    else
    {
    	return false;
    }
}

void drawBlockHelper(const TetrisBlockPropertiesTypeDef *block, int row, int col, uint16_t color, uint16_t outline_color)
{
	int pixel_x = block->x + col * block->cellsize; // calculate pixel coordinates
	int pixel_y = block->y + row * block->cellsize;
	if(outline_color == LCD_COLOR_WHITE)
	{
		LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize, LCD_COLOR_WHITE); //for white border around block
		LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize - 2, color);
	}
	if(outline_color == LCD_COLOR_BLACK)
	{
		LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize, LCD_COLOR_BLACK); //for white border around block
		LCD_Draw_Square_Fill(pixel_x + block->cellsize / 2, pixel_y + block->cellsize / 2, block->cellsize - 2, color);
	}
}

//RNG FUNCTIONS cus im a boss and dont wanna do them in applicationcode
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

