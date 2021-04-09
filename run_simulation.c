#include "sdl_grid.h"

static bool evaluate_cell(int i, int j, bool** scene);
static void render_box(int y, int x, SDL_Renderer* renderer);

void    run_simulation(bool** scene, SDL_Renderer *renderer) {
    bool** updated_scene = allocate_grid();

    for (int x = 0; x < 10; x++) {
        draw_grid(renderer);
        for (int i = 0; i < GRID_H; i++) {
            for (int j = 0; j < GRID_W; j++) {
                updated_scene[i][j] = evaluate_cell(i, j, scene);
                if (updated_scene[i][j]) {
                    render_box(i, j, renderer);
                }
            }
        }
    }
    return;
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

static bool evaluate_cell(int i, int j, bool** scene) {
    int alive_cells = 0;

    alive_cells = scene[i - 1][j - 1] + scene[i][j - 1] + scene[i + 1][j - 1] +
                  scene[i - 1][j] + scene[i + 1][j] +
                  scene[i - 1][j + 1] + scene[i][j + 1] + scene[i + 1][j + 1];
    return (alive_cells == 2 || alive_cells == 3);
}