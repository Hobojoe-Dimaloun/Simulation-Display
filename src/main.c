/*********************
* Date of creation: 02/11/17
* Author: Michael O'Donnell
* Contact: mo14776@my.bristol.ac.uk
* Other Authors:
**************************************
* Change History
**************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
static void close();

void screenRenderfunc(int *v, int x, int y);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

const double gBoltzmann = 1.38064852E-23;
const double gPi = 3.14159;

int main(int argc,char **argv)
{
    printf("Display Begin.\n");

    FILE *output=fopen("output.txt","r");
    if(output == NULL)
    {
        printf("Exit failure.\n");
        exit(EXIT_FAILURE);
    }

    int x; // x - dimension lattice sites
    int y; // y - dimension lattice sites
    int z = 1; // z - dimension latties sites

    fscanf(output,"%d",&x);
    fscanf(output,"%d",&y);

    printf("Ising model begin.\n");

    int *ising_lattice = NULL;

    //
    // Allocate lattice memory
    //

    if(( ising_lattice = calloc(x * y * z, sizeof(*ising_lattice) )) == NULL)
    {
        printf("-Error %d : %s\nFile : %s\nLine : %d\n", errno, strerror(errno), __FILE__, __LINE__);
    }

    //
    // initialise SDL
    //
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
        return 0;
    }


    bool quit = false;

    int loop = 0;
	//Event handler
	SDL_Event e;
	//While application is running
	while( !quit && (loop <= 100) )
	{
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT)
			{
				quit = true;
			}
		}


        for(int i = 0; i < x*y; i++)
        {
            fscanf(output,"%d",&ising_lattice[i]);
        }
        int sum ;
        fscanf(output,"%d",&sum);

        printf( "Magnetisation %d\n",sum);



        screenRenderfunc(ising_lattice,x,y);

        getchar();
        loop ++;
    }
    close();

    return 0;
}

void screenRenderfunc(int *v, int x, int y)
{
    const int pixelwidth = 5;
    const int pixelheight = 5;

	//While application is running
	//Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

	//Render red filled quad

    for(int i = 0; i < y; i ++)
    {
        for (int j = 0; j < x; j ++)
        {
            SDL_Rect fillRect = { j*pixelwidth, i*pixelheight, pixelwidth,pixelheight};
			if(v[i*x +j] == 1)
            {
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
            }
            else
            {
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
            }
            SDL_RenderFillRect( gRenderer, &fillRect );

        }
    }

	//Update screen
	SDL_RenderPresent( gRenderer );
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_SOFTWARE );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

static void close()
{
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}
