extern "C" {
    #include "sdl_grid.h"
}

int main() {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int*	scene;
	struct runtime_info info;
	cudaError_t error;

	if (get_run_mode(&info) == -1) {
		return -1;
	}
	if (SDL_Init(SDL_INIT_VIDEO) == -1){
		printf("Couldn't initialize SDL2: %s", SDL_GetError());
        return -1;
	}

	cudaMallocHost(&scene, sizeof(int) * GRID_W * GRID_H);
	if ((error = cudaGetLastError())) {
		printf("ERROR1: %s\n", cudaGetErrorString(error));
	}
	if (info.delay > 0) {
		if (SDL_CreateWindowAndRenderer(WINDOW_W, WINDOW_H, 0, &window, &renderer) == -1) {
			printf("Could not initialize window: %s", SDL_GetError());
			return -1;
		}
		init_world(scene, renderer);
	}

	clock_t t;
	t = clock();

	switch (info.device) {
		case CPU:
			run_simulation(scene, renderer, info.delay);
			cudaFree(scene); //Can't free in function since it is in C not CU
			break;
		case GPU:
			run_simulation_cuda(scene, renderer, info.delay);
			break;
		default:
			break;
	}
	t = clock() - t;
	double time = (double)t / CLOCKS_PER_SEC;
	printf("Time spent: %f\n", time);

	return 0;
}