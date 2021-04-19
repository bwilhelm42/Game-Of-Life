#ifndef SDL_GRID_H
#define SDL_GRID_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define CELL_SIZE 36
#define GRID_H 30
#define GRID_W 30
#define WINDOW_H (GRID_H * CELL_SIZE) + 1
#define WINDOW_W (GRID_W * CELL_SIZE) + 1

int**   init_world(SDL_Renderer *renderer);
void    run_simulation(int** scene, SDL_Renderer *renderer);
int**	allocate_grid();
void	draw_grid(SDL_Renderer *renderer);
void    free_2d_array(int x, int** array);

#endif