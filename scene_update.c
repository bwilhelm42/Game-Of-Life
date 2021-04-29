#include "sdl_grid.h"

static void render_box(int y, int x, SDL_Renderer* renderer);

void    scene_update(int *scene, SDL_Renderer *renderer) {
    draw_grid(renderer);
    for (int i = 0; i < GRID_H * GRID_W; i++) {
        if (scene[i]) {
            render_box(i / GRID_H, i % GRID_H, renderer);
        }
    }
    SDL_RenderPresent(renderer);
}

static void render_box(int y, int x, SDL_Renderer* renderer) {
	SDL_Color box_color = {.r = 240, .g = 240, .b = 240, .a = 240};
    SDL_Rect rect = {
        .h = CELL_SIZE - 1,
        .w = CELL_SIZE - 1,
        .x = x * CELL_SIZE,
        .y = y * CELL_SIZE
    };

    SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
    SDL_RenderFillRect(renderer, &rect);
}