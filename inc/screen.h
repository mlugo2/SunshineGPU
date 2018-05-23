#include <SDL2/SDL.h>
#include <stdio.h>

bool m_quit = false;

// Starts up SDL and creates window
bool init_screen();

// Shuts down SDL
void close_sdl();

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The window renderer
SDL_Renderer* gRenderer = NULL;



// The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

bool init_screen() {

	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else {

		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		// Create window
		gWindow = SDL_CreateWindow("GPU", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );

		if (gWindow == NULL) {
			printf("Window could not created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else {

			// Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
			if ( gRenderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				// Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00);
			}
		}
	}

	return success;
}

void close_sdl() {
	// Deallocate surface
	SDL_FreeSurface( gXOut );

	// Destroy window
	SDL_DestroyWindow( gWindow );

	// Quit SDL subsystem
	SDL_Quit();
}

void thread_screen( unsigned char FB[][SCREEN_WIDTH][3])
{
	// Event handler
	SDL_Event e;

	// Accessing each byte in vram
	int x, y;

	// Start up SDL and create window
	if ( !init_screen() ) {
		std::cout << "Failed to Initialize!\n";
	}
	else {
		// While application is running
		while( !m_quit ) {

			// Handle events on queue
			while( SDL_PollEvent(&e) != 0) {
				// User requests quit
				if ( e.type == SDL_QUIT ) { m_quit = true; }
			}

			// Clear screen
			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear( gRenderer );

			// Render to screen given frame buffer
			for (y = 0; y < SCREEN_HEIGHT; y++) {
				for (x = 0; x < SCREEN_WIDTH; x++) {

					// Render pixel
					SDL_Rect pixel = { x, y, 1, 1};

					// Draw to the screen
					SDL_SetRenderDrawColor( gRenderer, 
							FB[y][x][0], 
							FB[y][x][1], 
							FB[y][x][2], 
							255
						);
					SDL_RenderFillRect( gRenderer, &pixel );
				}
			}

			// Update screen
			SDL_RenderPresent( gRenderer );
		}

		// Free resources and close SDL
		close_sdl();
		
	}
}
