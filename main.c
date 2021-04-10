#include "sdl_grid.h"

int main() {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int** scene;

	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("Couldn't initialize SDL2: %s", SDL_GetError());
        return -1;
	}
	if (SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, 0, &window, &renderer) == -1) {
		printf("Could not initialize window: %s", SDL_GetError());
		return -1;
	}

	scene = init_world(renderer);
	if (scene == NULL) {
		return -1;
	}
	run_simulation(scene, renderer);
	return 1;
}