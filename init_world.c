#include "sdl_grid.h"

static void remove_from_scene(SDL_MouseButtonEvent event, int** scene);
static void	draw_boxes(SDL_Renderer *renderer, int** scene);
static void add_to_scene(SDL_MouseButtonEvent event, int** scene);

int**  init_world(SDL_Renderer *renderer) {
	SDL_Event event;
	bool quit = false;
	int **scene;

	scene = allocate_grid();

	while (quit == false){
		SDL_WaitEvent(&event);

		draw_grid(renderer);

		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_RETURN:
						return scene;
					case SDLK_ESCAPE:
						quit = true;
						break;
					default:
						break;
				}
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button) {
					case SDL_BUTTON_LEFT:
						add_to_scene(event.button, scene);
						break;
					case SDL_BUTTON_RIGHT:
						remove_from_scene(event.button, scene);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
		draw_boxes(renderer, scene);
		SDL_RenderPresent(renderer);
	}
	SDL_Quit();
	return NULL;
}

static void add_to_scene(SDL_MouseButtonEvent event, int** scene) {
	int x = event.x / CELL_SIZE;
	int y = event.y / CELL_SIZE;

	scene[y][x] = 1;
}

static void remove_from_scene(SDL_MouseButtonEvent event, int** scene) {
	int x = event.x / CELL_SIZE;
	int y = event.y / CELL_SIZE;

	scene[y][x] = 0;
}
static void	draw_boxes(SDL_Renderer *renderer, int** scene) {
	SDL_Color box_color = {.r = 240, .g = 240, .b = 240, .a = 240};
	SDL_Rect box = {
		.h = CELL_SIZE - 1,
		.w = CELL_SIZE - 1,
	};

	SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
	for (int i = 0; i < GRID_H; i++) {
		for (int j = 0; j < GRID_W; j++) {
			if (scene[i][j] == 1) {
				box.x = j * CELL_SIZE;
				box.y = i * CELL_SIZE;
				SDL_RenderFillRect(renderer, &box);
			}
		}
	}
}