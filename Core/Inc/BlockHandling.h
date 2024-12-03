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

#define GRID_HEIGHT 320
#define GRID_WIDTH 240

typedef struct
{
    uint16_t shape_rotations[4]; // 4 rotations
    uint16_t color;
} BlockShape;

//read only, declared here but defined in source file
extern const BlockShape T;
extern const BlockShape I;

typedef struct
{
	uint16_t block_color;
    uint16_t x, y;
    int cellsize;
    int rotation;    // current rotation
    const BlockShape *shapeType;   // I, T, etc
} TetrisBlockPropertiesTypeDef;


void drawBlock(const TetrisBlockPropertiesTypeDef *block);
void deleteBlock(const TetrisBlockPropertiesTypeDef *block);
void rotateBlock(TetrisBlockPropertiesTypeDef *block);
void moveBlockDown(TetrisBlockPropertiesTypeDef *block);
void moveBlockLeft(TetrisBlockPropertiesTypeDef *block);
void moveBlockRight(TetrisBlockPropertiesTypeDef *block);
bool detectCollision(const TetrisBlockPropertiesTypeDef *block, uint8_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize], int MOVE_X, int MOVE_Y);
void lockBlock(const TetrisBlockPropertiesTypeDef *block, uint8_t gameGrid[GRID_HEIGHT / block->cellsize][GRID_WIDTH / block->cellsize]);
void spawnBlock(TetrisBlockPropertiesTypeDef *block, const BlockShape *shapeType);

#endif /* INC_BLOCKHANDLING_H_ */
