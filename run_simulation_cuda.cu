extern "C" {
    #include "sdl_grid.h"
}

__global__ static void evaluate_cell(int* h_scene, int* updated_scene, int N);
static void render_box(int y, int x, SDL_Renderer* renderer);

extern "C" void    run_simulation_cuda(int* h_scene, SDL_Renderer *renderer, int delay) {
    int *updated_scene, *temp, *scene;
    int x = 0;
    int N = GRID_H * GRID_W;
    SDL_Event event;

    cudaMallocManaged(&updated_scene, GRID_H * GRID_W * sizeof(int));
    cudaMallocManaged(&scene, GRID_H * GRID_W * sizeof(int));
    cudaMemcpy(scene, h_scene, GRID_H * GRID_W * sizeof(int), cudaMemcpyHostToDevice);

    while (x++ < NUM_ITERATIONS) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_Quit();
                return;
            }
        }
        evaluate_cell<<<GRID_H, GRID_W>>>(scene, updated_scene, N);
        if (delay) {
            scene_update(updated_scene, renderer);
            SDL_Delay(delay);
        }
        temp = updated_scene;
        updated_scene = scene;
        scene = temp;
    }
    cudaFree(updated_scene);
    cudaFree(scene);
    free(h_scene);
    return;
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