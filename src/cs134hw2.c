/*

CS 134 Section 1 spring 2018 SJSU
Student: John Kennedy, 001462826
Program: HW2

Requirements: 1) make the 'player' tga move around the window using the keyboard. Make rate of speed constant across platforms. 
				2) Make a background for tile based game, make the game tile based. 

Extra credit: Make the player a different tga for the  direction in which it is  moving. 
Solution: Adding on to HW 1 and fixing the issues mentioned (my detection  of keys is a loop that eats up time). I have added a real 
		time timer to allow for  moving the glifs at a given rate of speed.  I rewrote program one to be more object oriented
		and easier to adapt to future plans. There are game piece objects now, and background and layer arrays that hold game pieces. 
		Also, I've switched to Warcraft pieces that I love. 

*/
#include "cs134hw2.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include"DrawUtils.h"
#include <stdlib.h>
#include <SDL.h>



int loadSprites(sprite* sprites[]) {
	/* loads the glifs from files.
	needs a glif array */

	/* get a list of all the glifs we need from disk */
	FILE* f = fopen("glifs.txt","r");
	char str[1024];
	int glifCount = 0;

	//for each glif named in the glif file
	while (fgets(str, 1024, f) != NULL)
	{
		if (strlen(str) == 0)  continue;

		char* pos;
		if ((pos = strchr(str, '\n')) != NULL)
		{
			*pos = '\0';
		}
	
		GLuint s;
		int sheight;
		int swidth; 
		s = glTexImageTGAFile(str,&sheight, &swidth);
		sprites[glifCount] = malloc(sizeof(sprite));
		sprites[glifCount]->spriteTGA = s;
		sprites[glifCount]->height = sheight;
		sprites[glifCount]->width = swidth;

		glifCount++;
	}
	fclose(f);
	return glifCount;

}
void createPlayers(gameboard* board)
{
	// relies on gameboard to already be malloced. 

	/* get a list of all the background positions and their glifs we need from disk */
	FILE* f = fopen("background.txt", "r");
	char str[1024];
	int lineCount = 0;
	int tileCount = 0;

	while (fgets(str, 1024, f) != NULL)
	{

		// get a line 
		if (strlen(str) == 0)  continue;

		//replace \n with \0
		char* pos;
		if ((pos = strchr(str, '\n')) != NULL)
		{
			*pos = '\0';
		}

		//break the line into tiles 
		char* tile = strtok(str, ",");
		int tile_pos = 0;
		while (tile != NULL)
		{
			//put the glif on the player. 
			int tileInt = atoi(tile);

			game_piece* p = malloc(sizeof(game_piece));
			// the cell it goes in. for the background layer. 
			int cell_x = tileCount % BOARD_WIDTH;
			int cell_y = tileCount / BOARD_WIDTH;

			p->speed = 0;
			p->sprite_index = tileInt;
			p->x = cell_x * CELL_WIDTH;
			p->y = cell_y * CELL_HEIGHT;
			
			board->bottom->pieces[cell_x][cell_y] = p;


			tile = strtok(NULL, ",");
			tile_pos++;
			tileCount++;
		}
	

		game_piece* footman = malloc(sizeof(game_piece));
		game_piece* grunt = malloc(sizeof(game_piece));
		footman->speed = 1; // ticks per pixel
		footman->x = 80;
		footman->y = 80;
		footman->sprite_index = 6;  //magic number - see glif file
		footman->last_move = 0;

		grunt->speed = 2; // half as fast as footman
		grunt->sprite_index = 9; //magic number - see glif file
		grunt->x = 100; //BOARD_WIDTH * CELL_WIDTH / 2;
		grunt->y = 100; //BOARD_HEIGHT * CELL_WIDTH / 2;
		grunt->last_move = 0;

		board->movers->pieces[0] = footman;
		board->movers->pieces[1] = grunt;




		
		lineCount++;
	}
	fclose(f);
	fprintf(stderr, "tile count = %d\n", tileCount);
}
void updatePieces(int direction[], moving_layer* l, sprite* sprites[]) {

	/*	for each piece in the stack, update position
		check for collision
		timeDiff is the amount of time  in ms that  has elapsed since the last refresh. 
		speed is in ticks per pixel 
	
	*/
	
	// first find the direction to which we will  move
	game_piece* footman = l->pieces[0];
	game_piece* grunt = l->pieces[1];

	int grunt_direction[2] = {0, 0};
	int grunt_y = grunt->y - footman->y;

	if (grunt_y < 0)
	{
		grunt_direction[0] = -1;
	}
	else if(grunt_y > 0)
	{
		grunt_direction[0] = 1;
	}
	else
	{
		grunt_direction[0] = 0;
	}
	int grunt_x = grunt->x - footman->x;

	if (grunt_x < 0)
	{
		grunt_direction[0] = -1;
	}
	else if (grunt_x > 0)
	{
		grunt_direction[0] = 1;
	}
	else
	{
		grunt_direction[0] = 0;
	}

	// test to see if it is  time to  move, and how much.
	// first, if this is the first pass, just move  on
	if (grunt->last_move + footman->last_move == 0)
	{
		grunt->last_move = SDL_GetTicks();
		footman->last_move = SDL_GetTicks();

	}

	// how far should grunt move if  at all

	int pixels_to_move = (SDL_GetTicks() - grunt->last_move) / grunt->speed;
	if (pixels_to_move != 0)
	{
		grunt->last_move += pixels_to_move * grunt->speed;
	}

	// how far should footman move

	pixels_to_move = (SDL_GetTicks() - footman->last_move) / footman->speed;
	if (pixels_to_move != 0)
	{
		footman->last_move += pixels_to_move * footman->speed;
	}
	
	// here we figure out  how not  to exceed our distance  limit when moving diagonally
	float dist_div = sqrt(pow(direction[0] , 2) + pow(direction[1],2));

}