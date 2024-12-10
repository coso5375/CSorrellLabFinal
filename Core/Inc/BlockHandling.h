/*
 * Blocks.h
 *
 *  Created on: Nov 27, 2024
 *      Author: connorsorrell
 */

#ifndef INC_BLOCKHANDLING_H_
#define INC_BLOCKHANDLING_H_

#include "LCD_Driver.h"
#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rng.h"

#define GRID_HEIGHT 320
#define GRID_WIDTH 240

#define NONE 0 //macros for moving block
#define MOVE_DOWN 1
#define MOVE_RIGHT 2
#define MOVE_LEFT 3
#define ROTATE 4


typedef struct
{
    uint16_t shape_rotations[4]; // 4 rotationsyy
    uint16_t color;
} BlockShape;

typedef struct
{
	uint16_t block_color;
    int x, y;
    int cellsize;
    int rotation;    // current rotation
    const BlockShape *shapeType;   // I, T, etc
} TetrisBlockPropertiesTypeDef;

//with a few edits, gamegrid is modular and cells can be bigger/smaller, hence gameGrids intiliatizion
void drawBlock(const TetrisBlockPropertiesTypeDef *block);
void deleteBlock(const TetrisBlockPropertiesTypeDef *block);
bool detectCollision(const TetrisBlockPropertiesTypeDef *block, uint16_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize], int MOVE_X, int MOVE_Y);
void lockBlock(const TetrisBlockPropertiesTypeDef *block, uint16_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize]);
void spawnBlock(TetrisBlockPropertiesTypeDef *block, uint32_t blockType);
void DisplayBlocks();
void drawBlockBlackOutline(const TetrisBlockPropertiesTypeDef *block);
uint8_t getRowBits(uint16_t shapeData, int row);
bool isCellInShape(uint8_t rowBits, int col);
void moveBlock(int MOVE, TetrisBlockPropertiesTypeDef *block);

//RNG functions
void RNG_Init();
uint32_t GenerateRandomNum();

#endif /* INC_BLOCKHANDLING_H_ */
