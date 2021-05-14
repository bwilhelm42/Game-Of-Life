#ifndef SDL_GRID_H
#define SDL_GRID_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define CELL_SIZE 25
#define GRID_H 4000
#define GRID_W 4000
#define WINDOW_H (GRID_H * CELL_SIZE) + 1
#define WINDOW_W (GRID_W * CELL_SIZE) + 1

#define NUM_ITERATIONS 20

#define GPU 0
#define CPU 1

struct runtime_info {
    int device;
    int delay;
};

void    init_world(int* scene, SDL_Renderer *renderer);
int     get_run_mode(struct runtime_info *info);
void    run_simulation(int* scene, SDL_Renderer *renderer, int delay);
void    run_simulation_cuda(int* scene, SDL_Renderer *renderer, int delay);
int**	allocate_grid();
void	draw_grid(SDL_Renderer *renderer);
void    free_2d_array(int x, int** array);
void    scene_update(int *scene, SDL_Renderer *renderer);

#endif