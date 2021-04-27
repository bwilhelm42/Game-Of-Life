#include "sdl_grid.h"

static int evaluate_cell(int i, int j, int* scene);
static void render_box(int y, int x, SDL_Renderer* renderer);

void    run_simulation(int* scene, SDL_Renderer *renderer, int delay) {
    int* updated_scene;
    int* temp;
    bool scene_change = true;
    SDL_Event event;
    int x = 0;

    updated_scene = (int*)malloc(sizeof(int) * GRID_H * GRID_W);
    bzero(updated_scene, GRID_W * GRID_H * sizeof(int));

    while (scene_change && x++ < 1000000) {
    printf("%d\n", scene[GRID_W + 1]);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_Quit();
                return;
            }
        }
        scene_change = false;
        draw_grid(renderer);
        for (int i = 0; i < GRID_H * GRID_W; i++) {
            updated_scene[i] = evaluate_cell(i / GRID_H, i % GRID_W, scene);
            if (updated_scene[i] != scene[i]) {
                scene_change = true;
            }
            if (updated_scene[i]) {
                render_box(i / GRID_H, i % GRID_W, renderer);
            }
        }
        temp = updated_scene;
        updated_scene = scene;
        scene = temp;
        SDL_RenderPresent(renderer);
        SDL_Delay(delay);
    }
    free(scene);
    free(updated_scene);
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

static int evaluate_cell(int i, int j, int* scene) {
    int alive_cells = 0;

    alive_cells = 
    (i > 0 && j > 0 && scene[(i - 1) * GRID_H + (j - 1)]) +
    (j > 0 && scene[i * GRID_H + (j - 1)]) +
    (i < GRID_H - 1 && j > 0 && scene[(i + 1) * GRID_H + (j - 1)]) +

    (i > 0 && scene[(i - 1) * GRID_H + j]) +
    (i < GRID_H - 1 && scene[(i + 1) * GRID_H + j]) +

    (i > 0 && j < GRID_W - 1 && scene[(i - 1) * GRID_H + (j + 1)]) +
    (j < GRID_W - 1 && scene[i * GRID_H + (j + 1)]) +
    (i < GRID_H - 1 && j < GRID_W - 1 && scene[(i + 1) * GRID_H + (j + 1)]);
    return (alive_cells == 3 || (alive_cells == 2 && scene[i * GRID_H + j] == 1));
}