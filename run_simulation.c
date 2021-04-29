#include "sdl_grid.h"

static int evaluate_cell(int i, int j, int* scene);

void    run_simulation(int* scene, SDL_Renderer *renderer, int delay) {
    int* updated_scene;
    int* temp;
    bool scene_change = true;
    SDL_Event event;
    int x = 0;

    updated_scene = (int*)malloc(sizeof(int) * GRID_H * GRID_W);

    while (scene_change && x++ < NUM_ITERATIONS) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_Quit();
                return;
            }
        }
        scene_change = false;
        for (int i = 0; i < GRID_H * GRID_W; i++) {
            updated_scene[i] = evaluate_cell(i / GRID_H, i % GRID_W, scene);
            if (updated_scene[i] != scene[i]) {
                scene_change = true;
            }
        }
        if (delay) {
            scene_update(updated_scene, renderer);
            SDL_Delay(delay);
        }
        temp = updated_scene;
        updated_scene = scene;
        scene = temp;
    }
    free(scene);
    free(updated_scene);
    return;
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