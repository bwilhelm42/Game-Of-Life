extern "C" {
    #include "sdl_grid.h"
}

__global__ static void evaluate_cell(int* h_scene, int* updated_scene, int N);
static void render_box(int y, int x, SDL_Renderer* renderer);

extern "C" void    run_simulation_cuda(int* h_scene, SDL_Renderer *renderer, int delay) {
    int *updated_scene, *temp, *scene;
    int x = 0;
    int N = GRID_H * GRID_W;

    cudaMallocManaged(&updated_scene, GRID_H * GRID_W * sizeof(int));
    cudaMallocManaged(&scene, GRID_H * GRID_W * sizeof(int));
    cudaMemcpy(scene, h_scene, GRID_H * GRID_W * sizeof(int), cudaMemcpyHostToDevice);

    while (x++ < 1000000) {
        evaluate_cell<<<GRID_H, GRID_W>>>(scene, updated_scene, N);
        draw_grid(renderer);
        for (int i = 0; i < N; i++) {
            if (scene[i]) {
                render_box(i / GRID_H, i % GRID_H, renderer);
            }
        }
        SDL_RenderPresent(renderer);
        temp = updated_scene;
        updated_scene = scene;
        scene = temp;
    }
    cudaFree(updated_scene);
    cudaFree(scene);
    free(h_scene);
    return;
}

static void render_box(int y, int x, SDL_Renderer* renderer) {
	SDL_Color box_color = {.r = 240, .g = 240, .b = 240, .a = 240};
    SDL_Rect rect;
    
    rect.h = CELL_SIZE - 1;
    rect.w = CELL_SIZE - 1;
    rect.x = x * CELL_SIZE;
    rect.y = y * CELL_SIZE;

    SDL_SetRenderDrawColor(renderer, box_color.r, box_color.g, box_color.b, box_color.a);
    SDL_RenderFillRect(renderer, &rect);
}

__global__
static void evaluate_cell(int* scene, int* updated_scene, int N) {
    int alive_cells = 0;
    int i = blockIdx.x;
    int j = threadIdx.x;
    
    alive_cells = 
    (i > 0 && j > 0 && scene[(i - 1) * GRID_H + (j - 1)]) +
    (j > 0 && scene[i * GRID_H + (j - 1)]) +
    (i < GRID_H - 1 && j > 0 && scene[(i + 1) * GRID_H + (j - 1)]) +

    (i > 0 && scene[(i - 1) * GRID_H + j]) +
    (i < GRID_H - 1 && scene[(i + 1) * GRID_H + j]) +

    (i > 0 && j < GRID_W - 1 && scene[(i - 1) * GRID_H + (j + 1)]) +
    (j < GRID_W - 1 && scene[i * GRID_H + (j + 1)]) +
    (i < GRID_H - 1 && j < GRID_W - 1 && scene[(i + 1) * GRID_H + (j + 1)]);
    updated_scene[i * GRID_W + j] = (alive_cells == 3 || (alive_cells == 2 && scene[i * GRID_H + j] == 1));
}