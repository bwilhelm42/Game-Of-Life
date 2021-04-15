#include "sdl_grid.h"

static int evaluate_cell(int i, int j, int** scene);
static void render_box(int y, int x, SDL_Renderer* renderer);

void    run_simulation(int** scene, SDL_Renderer *renderer) {
    int** updated_scene = allocate_grid();
    int** temp;
    bool scene_change = true;
    SDL_Event event;

    while (scene_change) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_Quit();
                return;
            }
        }
        scene_change = false;
        draw_grid(renderer);
        for (int i = 0; i < GRID_H; i++) {
            for (int j = 0; j < GRID_W; j++) {
                updated_scene[i][j] = evaluate_cell(i, j, scene);
                if (updated_scene[i][j] != scene[i][j]) {
                    scene_change = true;
                }
                if (updated_scene[i][j]) {
                    render_box(i, j, renderer);
                }
            }
        }
        temp = updated_scene;
        updated_scene = scene;
        scene = temp;
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
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

static int evaluate_cell(int i, int j, int** scene) {
    int alive_cells = 0;

    alive_cells = 
    (i > 0 && j > 0 && scene[i - 1][j - 1]) +
    (j > 0 && scene[i][j - 1]) +
    (i < GRID_H - 1 && j > 0 && scene[i + 1][j - 1]) +

    (i > 0 && scene[i - 1][j]) +
    (i < GRID_H - 1 && scene[i + 1][j]) +

    (i > 0 && j < GRID_W && scene[i - 1][j + 1]) +
    (j < GRID_W - 1 && scene[i][j + 1]) +
    (i < GRID_H - 1 && j < GRID_W - 1 && scene[i + 1][j + 1]);
    return (alive_cells == 3 || (alive_cells == 2 && scene[i][j] == 1));
}