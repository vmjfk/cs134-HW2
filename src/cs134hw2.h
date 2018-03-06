#pragma once
#include<GL/glew.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 576
#define BOARD_WIDTH 25
#define BOARD_HEIGHT 18
#define BOARD_LAYER_COUNT 2
#define CELL_WIDTH 32
#define CELL_HEIGHT 32
#define MAX_SPRITES 11
#define MOVING_PIECES 2



typedef struct sprite {
	GLuint spriteTGA;
	int width;
	int height;
} sprite;
typedef struct game_piece {
	int sprite_index;
	int x;
	int y;
	int speed; //ms per pixel;
	int last_move; //time when we last moved. 
} game_piece;

typedef struct board_layer {
	game_piece* pieces[BOARD_WIDTH][BOARD_HEIGHT];
	

}board_layer;
typedef struct moving_layer {
	game_piece* pieces[MOVING_PIECES];
} moving_layer;
typedef struct gameboard {
	board_layer* bottom;
	moving_layer* movers;

}gameboard;

int loadSprites(sprite* sprites[]);
void createPlayers(gameboard* board);
void updatePieces(int direction[], moving_layer* l, sprite* sprites[]);



