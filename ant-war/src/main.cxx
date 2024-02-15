

#ifdef _WIN32
#include <SDL.h>

#define not !

#else

#include <SDL2/SDL.h>

#endif


#include "Boid.h"
#include "it_s_work.h"
#include "Exception.cpp"
#include <iostream>
#include <random>
#include <vector>
#include <cmath>

int const WIDTH = 1280;
int const HEIGHT = 720;
//int const SCALE = 20; 
double const WIDTH_D = 1280.; // / SCALE;
double const HEIGHT_D = 720.; // / SCALE;
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


double inverse(double x) {
	if (((x > 0) && (x < 0.15)) || ((x > -0.15) && (x < 0))) {
		throw Exception ("division par zero");
	}
	else {
		return 1 / x;
	}
}

void paint_triangle(SDL_Renderer* renderer, double x1, double y1, double x2, double y2, double x3, double y3) { 
	double miny = y1;
	double xminy = x1;
	double maxy = y3;
	double xmaxy = x3;
	double inty = y2;
	double xinty = x2;
	//On cherche le minimum
	if (miny > inty) {
		std::swap(miny, inty);
		std::swap(xminy, xinty);
	};
	if (inty > maxy) {
		std::swap(inty, maxy);
		std::swap(xinty, xmaxy);
	};
	if (miny > inty) {
		std::swap(miny, inty);
		std::swap(xminy, xinty);
	};
	// Parcourir chaque ligne verticale du triangle
	double xmin;
	double xmax;
	for (int y = miny; y <= inty; y++) {
		try { xmin = xminy + (xinty - xminy) * (y - miny) * inverse(inty - miny); }
		catch (Exception&) {
			xmin = xinty;
		};
		try { xmax = xminy + (xmaxy - xminy) * (y - miny) * inverse(maxy - miny); }
		catch (Exception&) {
			xmax = xminy;
		}; 
		if (xmin > xmax) {
			std::swap(xmin, xmax);
		}
		for (int x = xmin; x <= xmax; x++) {
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	for (int y = inty; y <= maxy; y++) {
		try { xmin = xinty + (xmaxy - xinty) * (y - inty) * inverse(maxy - inty); }
		catch (Exception&) {
			xmin = xinty;
		};
		try { xmax = xminy + (xmaxy - xminy) * (y - miny) * inverse(maxy - miny); }
		catch (Exception&) {
			xmax = xmaxy;
		};
		if (xmin > xmax) {
			std::swap(xmin, xmax);
		}
		for (int x = xmin; x <= xmax; x++) {
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}




void paint_boid(Boid boid, int taille) {
	SDL_SetRenderDrawColor(g.renderer, boid.get_color_r(), boid.get_color_g(), boid.get_color_b(), SDL_ALPHA_OPAQUE);
	Pair<double> p = boid.get_pos();
	double p_x = p.get_first();
	double p_y = p.get_second();
	Pair<double> s = boid.get_speed();
	Pair<double> dir = s / s.module();
	double dir_x = dir.get_first();
	double dir_y = dir.get_second();
	paint_triangle(g.renderer, p_x + dir_x * taille, p_y + dir_y * taille, p_x - dir_x * taille / 5, p_y + dir_y * taille / 5, p_x + dir_x * taille / 5, p_y - dir_y * taille / 5);
}


void paint_boids(Group * my_group) {
	Boid* tab_prey = (*my_group).get_tab_prey();
	Boid* tab_predator = (*my_group).get_tab_predator();
	for (int i = 0; i < (*my_group).get_num_prey(); ++i) {
		paint_boid(tab_prey[i], 10);
	}
	for (int i = 0; i < (*my_group).get_num_predator(); ++i) {
		paint_boid(tab_predator[i], 20);
	}
}

void do_render(Group* my_group) {
	SDL_SetRenderDrawColor(g.renderer, 255u, 255u, 255u, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(g.renderer);
	paint_boids(my_group);
	SDL_RenderPresent(g.renderer);
}

int main(int argc, char ** argv)
{
	Group my_group (150, 5, HEIGHT_D, WIDTH_D);

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

