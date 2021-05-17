extern "C" {
    #include "sdl_grid.h"
}

int     calculate_stream_dim(int num_streams, int N);
void    generate_streams(cudaStream_t *streams, int stream_count);
__global__ static void evaluate_cell(int* h_scene, int* updated_scene, int N);

extern "C" void    run_simulation_cuda(int* h_scene, SDL_Renderer *renderer, int delay) {
    int *updated_scene, *temp, *scene;
    int x = 0;
    int N = GRID_H * GRID_W;
    SDL_Event event;
    cudaError_t error;
    cudaStream_t *streams = NULL;
    int index, data_size;
    int num_streams = 1;
    int stream_dim = calculate_stream_dim(num_streams, N);

    generate_streams(streams, num_streams);

    cudaMallocManaged(&updated_scene, GRID_H * GRID_W * sizeof(int));
    if ((error = cudaGetLastError())) {
        printf("ERROR: %s\n", cudaGetErrorString(error));
    }
    cudaMallocManaged(&scene, GRID_H * GRID_W * sizeof(int));
    cudaMemcpy(scene, h_scene, GRID_H * GRID_W * sizeof(int), cudaMemcpyHostToDevice);

    while (x++ < NUM_ITERATIONS) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
                SDL_Quit();
                return;
            }
        }
        for (int i = 0; i < num_streams; i++) {
            index = stream_dim * (i + 1);
            data_size = index > N ? index - N : stream_dim;
            printf("%d\n", (&(scene[i * stream_dim]))[i * stream_dim]);
            printf("Running stream %d\nData size: %d\n", i, data_size);
            evaluate_cell<<<128, 128, 0, streams[i]>>>(scene, updated_scene, data_size);
            printf("made it\n");
        }
        cudaDeviceSynchronize();
        cudaMemcpy(h_scene, updated_scene, sizeof(int) * GRID_H * GRID_W, cudaMemcpyDeviceToHost);
        if (delay) {
            scene_update(h_scene, renderer);
            SDL_Delay(delay);
        }
        temp = scene;
        scene = updated_scene;
        updated_scene = temp;
    }
    cudaFree(updated_scene);
    cudaFree(scene);
    cudaFree(h_scene);
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

void    generate_streams(cudaStream_t *streams, int stream_count) {
    streams = (cudaStream_t*)malloc(sizeof(cudaStream_t) * stream_count);
    for (int i = 0; i < stream_count; i++) {
        cudaStreamCreate(&(streams[i]));
    }
}

int     calculate_stream_dim(int num_streams, int N) {
    int upper, lower, dim;

    upper = N / num_streams;
    lower = N - (upper * num_streams);
    dim = upper + lower;
    return dim;
}