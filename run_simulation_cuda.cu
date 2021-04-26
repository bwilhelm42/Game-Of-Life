#include "sdl_grid.h"

static int evaluate_cell(int i, int j, int* scene);
static void render_box(int y, int x, SDL_Renderer* renderer);

void    run_simulation_cuda(int* scene, SDL_Renderer *renderer, int delay) {
    int* updated_scene, temp;
    int x = 0;
    int N = GRID_H * GRID_W;

    cudaMallocManaged(updated_scene, GRID_H * GRID_W * sizeof(int));

    while (x++ < 100000) {
        evaluate_cell<<<1,1>>>(scene, updated_scene, N);
        temp = updated_scene;
        updated_scene = scene;
        scene = temp;
    }
    free_2d_array(GRID_H, scene);
    free_2d_array(GRID_H, updated_scene);
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

__global__
static int evaluate_cell(int* scene, int** updated_scene, int N) {
    int alive_cells = 0;

    alive_cells = 
    (i > 0 && j > 0 && scene[(i - 1) * GRID_H + (j - 1)]) +
    (j > 0 && scene[i * GRID_H + (j - 1)]) +
    (i < GRID_H - 1 && j > 0 && scene[(i + 1) * GRID_H + (j - 1)]) +

    (i > 0 && scene[i - 1][j]) +
    (i < GRID_H - 1 && scene[(i + 1) * GRID_H + j]) +

    (i > 0 && j < GRID_W - 1 && scene[(i - 1) * GRID_H + (j + 1)]) +
    (j < GRID_W - 1 && scene[i * GRID_H + (j + 1)]) +
    (i < GRID_H - 1 && j < GRID_W - 1 && scene[(i + 1) * GRID_H + (j + 1)]);
    updated_scene[i * GRID_W + j] = (alive_cells == 3 || (alive_cells == 2 && scene[i * GRID_H + j] == 1));
}