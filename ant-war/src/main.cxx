

#ifdef _WIN32
#include <SDL.h>

#define not !

#else

#include <SDL2/SDL.h>

#endif


#include "Boid.h"
#include "it_s_work.h"
#include <iostream>
#include <random>
#include <vector>
#include <cmath>

int const WIDTH = 1280;
int const HEIGHT = 720;
int const SCALE = 20; 
double const WIDTH_D = 1280. / SCALE;
double const HEIGHT_D = 720. / SCALE;
float const PI = 3.1415927; // TODO: better PI

struct global_t {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;

	// random
	std::random_device rd;
	std::default_random_engine eng;
	std::uniform_real_distribution<float> rand;

};

global_t g;
/*
void paint_it_s_work(int ox, int oy, int scale = 20) {
	SDL_SetRenderDrawColor(g.renderer, 0u, 0u, 0u, SDL_ALPHA_OPAQUE);
	for (int j = 0; j < px::height; ++j) {
		for (int i = 0; i < px::width; ++i) {
			if (px::header_data[j*px::width+i] == 0) {
				SDL_Rect r = { i*scale+ox, j*scale+oy, 20, 20 };
				SDL_RenderFillRect(g.renderer, &r);
			}
		}
	}
}
*/

void paint_boid(Group * my_group, int ox, int oy, int scale = 20) {
	SDL_SetRenderDrawColor(g.renderer, 0u, 0u, 0u, SDL_ALPHA_OPAQUE);
	Boid* tab = (*my_group).Get_Tab();
	for (int i = 0; i < (*my_group).Get_Num(); ++i) {
		Pair<double> p = tab[i].Get_Pos();
		SDL_Rect r = { p.get_First() * scale + ox, p.get_Second() * scale + oy, 5, 5 };
		SDL_RenderFillRect(g.renderer, &r);
	}
}

void do_render(Group* my_group) {
	SDL_SetRenderDrawColor(g.renderer, 255u, 255u, 255u, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(g.renderer);
	paint_boid(my_group, 0, 0, SCALE);
	SDL_RenderPresent(g.renderer);
}

int main(int argc, char ** argv)
{
	Group my_group (100, HEIGHT_D, WIDTH_D);

	int status;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		return 1;
	}

	g.window = SDL_CreateWindow("Ant War",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (not g.window) {
		return 1;
	}

	// get the default renderer
	g.renderer = SDL_CreateRenderer(g.window, -1, 0);
	if (not g.renderer) {
		return 1;
	}

	bool end = false;
	while (not end) {
		SDL_Event event;
		if (SDL_WaitEventTimeout(&event, 20)) {
			switch (event.type) {
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
						end = true;
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						// Should never happen
						break;
					default:
						break;
				}
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					end = true;
				}
				break;
			case SDL_KEYUP:
				break;
			}
		} else {
			// Got time out or error
			char const * e = SDL_GetError();
			if (e != NULL) {
				if (strlen(e) != 0) {
					// Got error
					return 1;
				}
			}

			my_group.new_group(HEIGHT_D, WIDTH_D);
			do_render(& my_group);
		}
	}

	SDL_DestroyRenderer(g.renderer);
	SDL_DestroyWindow(g.window);
	SDL_CloseAudio();
	SDL_Quit();
	return 0;
}

