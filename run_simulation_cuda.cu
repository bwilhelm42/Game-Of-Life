extern "C" {
    #include "sdl_grid.h"
}

static void my_swap(int **a, int **b);
__global__ static void evaluate_cell(int* h_scene, int* updated_scene, int N);

extern "C" void    run_simulation_cuda(int* h_scene, SDL_Renderer *renderer, int delay) {
    int *updated_scene, *scene;
    int x = 0;
    int N = GRID_H * GRID_W;
    SDL_Event event;
    cudaError_t error;

    cudaMalloc(&updated_scene, GRID_H * GRID_W * sizeof(int));
    cudaMalloc(&scene, GRID_H * GRID_W * sizeof(int));
    cudaMemcpy(scene, h_scene, GRID_H * GRID_W * sizeof(int), cudaMemcpyHostToDevice);

    while (x++ < NUM_ITERATIONS) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_Quit();
                return;
            }
        }
        evaluate_cell<<<GRID_W, GRID_H>>>(scene, updated_scene, N);
        cudaMemcpy(h_scene, updated_scene, sizeof(int) * GRID_H * GRID_W, cudaMemcpyDeviceToHost);
        if ((error = cudaGetLastError())) {
            printf("ERROR: %s\n", cudaGetErrorString(error));
        }
        if (delay) {
            scene_update(h_scene, renderer);
            SDL_Delay(delay);
        }
        my_swap(&scene, &updated_scene);
    }
    cudaFree(updated_scene);
    cudaFree(scene);
    return;
}

__global__
static void evaluate_cell(int* scene, int* updated_scene, int N) {
    int alive_cells = 0;
    int i = blockIdx.x;
    int j = threadIdx.x;
    
    for (int k = i * blockDim.x + j; k < N; k += blockDim.x * gridDim.x) {

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
}

static void my_swap(int **a, int **b) {
    int *c;

    c = *a;
    *a = *b;
    *b = c;
}