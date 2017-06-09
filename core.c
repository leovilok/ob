#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum keys {
	LEFT,
	RIGHT,
	UP,
	DOWN,
	A,
	B,
	RETURN,
	ESCAPE,
};

/* api */
extern void init();
extern void loop();

/* global data */
char pixels[256*256];
unsigned char input;

void show_FPS(unsigned iter){
	static unsigned oldtime;
	static unsigned newtime;

	if(iter%64 == 0){
		oldtime = newtime;
		newtime = SDL_GetTicks();
		fprintf(stderr,"fps %.3f  \r", 1000.0/((newtime-oldtime)/64.0));
	}
}

int get_events(void){
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT
		    || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
			return 0;
		}

		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					input |= 1<<LEFT;
					break;

				case SDLK_RIGHT:
					input |= 1<<RIGHT;
					break;

				case SDLK_UP:
				        input |= 1<<UP;
					break;

				case SDLK_DOWN:
					input |= 1<<DOWN;
					break;

				case SDLK_a:
					input |= 1<<A;
					break;

				case SDLK_z:
					input |= 1<<B;
					break;

				case SDLK_RETURN:
					input |= 1<<RETURN;
					break;

				case SDLK_ESCAPE:
					input |= 1<<ESCAPE;
					break;
			}
		}
		if (e.type == SDL_KEYUP) {
			switch (e.key.keysym.sym) {
				case SDLK_LEFT:
					input &= ~(1<<LEFT);
					break;

				case SDLK_RIGHT:
					input &= ~(1<<RIGHT);
					break;

				case SDLK_UP:
				        input &= ~(1<<UP);
					break;

				case SDLK_DOWN:
					input &= ~(1<<DOWN);
					break;

				case SDLK_a:
					input &= ~(1<<A);
					break;

				case SDLK_z:
					input &= ~(1<<B);
					break;

				case SDLK_RETURN:
					input &= ~(1<<RETURN);
					break;

				case SDLK_ESCAPE:
					input &= ~(1<<ESCAPE);
					break;
			}
		}

	}

	return 1;
}

int main(){
	// init
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_Window *window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256, 256, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, 256, 256);
	
	SDL_Rect rect = {.x=0,.y=0,.w=256,.h=256};
	
	init();

	void *pix;
	int pitch;
	SDL_RenderClear(renderer);
	SDL_LockTexture(texture, &rect, &pix, &pitch);
	memcpy(pix, pixels, 256*256);
	SDL_UnlockTexture(texture);


	puts("inited");

	// loop
	unsigned iter = 0;
	int cont = 1;
	while(cont) {
//		show_FPS(iter++);

		cont = get_events();
		
		fprintf(stderr,"input %3u \r", (unsigned) input);

		loop();

		SDL_LockTexture(texture, &rect, &pix, &pitch);
		memcpy(pix, pixels, 256*256);
		SDL_UnlockTexture(texture);

		SDL_RenderCopy(renderer, texture, &rect, &rect);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}

	// finish
	puts("\nfinish");

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
