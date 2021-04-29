#include "sdl_grid.h"

int main() {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int*	scene;
	struct runtime_info info;

	if (get_run_mode(&info) == -1) {
		return -1;
	}
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("Couldn't initialize SDL2: %s", SDL_GetError());
        return -1;
	}
	if (SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, 0, &window, &renderer) == -1) {
		printf("Could not initialize window: %s", SDL_GetError());
		return -1;
	}

	if ((scene = init_world(renderer)) == NULL) {
		return -1;
	}

	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	switch (info.device) {
		case CPU:
			run_simulation(scene, renderer, info.delay);
			break;
		case GPU:
			run_simulation_cuda(scene, renderer, info.delay);
			break;
		default:
			break;
	}

	clock_gettime(CLOCK_REALTIME, &end);
	double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
	printf("Total simulation runtime: %f\n", time_spent);
	return 1;
}