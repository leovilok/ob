#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define CHECK_DLERROR(ptr) do\
	if(!ptr){\
		fprintf(stderr, "%s\n", dlerror());\
		exit(1);\
	} while (0)

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

/* global data */
unsigned char pixels[256*256];
unsigned char input;

float FPS;

void update_FPS(unsigned iter){
	static unsigned oldtime;
	static unsigned newtime;

	if(iter%64 == 0){
		oldtime = newtime;
		newtime = SDL_GetTicks();
		FPS = 1000.0/((newtime-oldtime)/64.0);
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

int main(int argc, char *argv[]){
	// init
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_Window *window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256, 256, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, 256, 256);
	
	SDL_Rect rect = {.x=0,.y=0,.w=256,.h=256};
	
	if(argc != 2){
		fprintf(stderr, "Usage: %s ./file.so\n", argv[0]);
		exit(1);
	}
	
	void *handle = dlopen(argv[1], RTLD_LAZY);
	CHECK_DLERROR(handle);

	void (*init)(unsigned char*, unsigned char*) = dlsym(handle, "init");
	CHECK_DLERROR(init);

	void (*loop)() = dlsym(handle, "loop");
	CHECK_DLERROR(loop);

	init((unsigned char*)&pixels, &input);

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
		update_FPS(iter);

		cont = get_events();
		
		fprintf(stderr,"input %3u, FPS %3.3f\r", (unsigned) input, FPS);

		loop();

		SDL_LockTexture(texture, &rect, &pix, &pitch);
		memcpy(pix, pixels, 256*256);
		SDL_UnlockTexture(texture);

		SDL_RenderCopy(renderer, texture, &rect, &rect);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);

		iter++;
	}

	// finish
	puts("\nfinish");

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
