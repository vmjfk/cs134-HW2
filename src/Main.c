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




#define SDL_MAIN_HANDLED


#include<SDL.h>
#include<GL/glew.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include"DrawUtils.h"
#include"cs134hw2.h"

// Set this to true to make the game loop exit.
char shouldExit = 0;

// The previous frame's keyboard state.
unsigned char kbPrevState[SDL_NUM_SCANCODES] = {0};

// The current frame's keyboard state.
const unsigned char* kbState = NULL;

int main(void)
{
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
        return 1;
    }

    // Create the window and OpenGL context.
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_Window* window = SDL_CreateWindow(
            "SDLTemplate",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_OPENGL);
    if (!window) {
        fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_GL_CreateContext(window);

    // Make sure we have a recent version of OpenGL.
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
        SDL_Quit();
        return 1;
    }
    if (GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 or greater supported: Version=%s\n",
                 glGetString(GL_VERSION));
    } else {
        fprintf(stderr, "OpenGL max supported version is too low.\n");
        SDL_Quit();
        return 1;
	}

	// Setup OpenGL state.
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Game initialization goes here.
	Uint32 prev_loop_time = SDL_GetTicks();

	sprite* sprites[MAX_SPRITES];
	gameboard* board = malloc(sizeof(gameboard));

	board->bottom = malloc(sizeof(board_layer));
	board->movers = malloc(sizeof(moving_layer));


	loadSprites(sprites); // get the glifs loaded into GLEW memory

	createPlayers(board);


	// The game loop.
	kbState = SDL_GetKeyboardState(NULL);

	while (!shouldExit) {


		// other init code

		assert(glGetError() == GL_NO_ERROR);
		memcpy(kbPrevState, kbState, sizeof(kbPrevState));

		// Handle OS message pump.
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				shouldExit = 1;
			}
		}

		// Game logic goes here.
		if (kbState[SDL_SCANCODE_ESCAPE]) {
			shouldExit = 1;
		}

		// get the direction that the keyboard is  indicating. 
		int direction[2];
		direction[0] = direction[1] = 0;


		if (kbState[SDL_SCANCODE_RIGHT])
		{
			direction[0]++;
		}
		if (kbState[SDL_SCANCODE_LEFT]) {
			direction[0]--;
		}
		if (kbState[SDL_SCANCODE_DOWN]) {
			direction[1]++;
		}
		if (kbState[SDL_SCANCODE_UP]) {
			direction[1]--;
		}

		// update our player positions. 
		updatePieces(direction, board->movers,sprites);
		


		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Game drawing goes here.
		// draw the background

		for (int i = 0; i < (BOARD_HEIGHT*BOARD_WIDTH); i++)
		{
			int cell_x = i % BOARD_WIDTH;
			int cell_y = i / BOARD_WIDTH;
			game_piece* p = board->bottom->pieces[cell_x][cell_y];
			sprite* s = sprites[p->sprite_index];

			glDrawSprite(s->spriteTGA, p->x, p->y, s->height, s->width);
		}

		// draw the players. 
		for (int i = 0; i < MOVING_PIECES; i++)
		{

			game_piece* p = board->movers->pieces[i];
			sprite* s = sprites[p->sprite_index];
			fprintf(stderr, "p->sprite_index = %d\n", p->sprite_index);
			glDrawSprite(s->spriteTGA, p->x, p->y, s->height, s->width);
		}
			

		
		// Present the most recent frame.
        SDL_GL_SwapWindow(window);
		
	
    }

    SDL_Quit();

    return 0;
}
