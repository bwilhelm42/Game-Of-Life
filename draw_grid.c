#include "sdl_grid.h"

void	draw_grid(SDL_Renderer *renderer) {
	SDL_Color bg_color = {.r = 22, .g = 22, .b = 22, .a = 255};
	SDL_Color line_color = {.r = 44, .g = 44, .b = 44, .a = 255};

	SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, line_color.r, line_color.g, line_color.b, line_color.a);
	for (int x = 0; x < WINDOW_W; x += CELL_SIZE) {
		SDL_RenderDrawLine(renderer, x, 0, x, WINDOW_H);
	}
	for (int y = 0; y < WINDOW_H; y += CELL_SIZE) {
		SDL_RenderDrawLine(renderer, 0, y, WINDOW_W, y);
	}
}