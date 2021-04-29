#include "sdl_grid.h"

int get_run_mode(struct runtime_info *info) {
    static bool i = false;
    char c;
    int delay;

    if (!i) {
        printf("Welcome to Conway's Game of Life\nPlease select an operating mode\n");
        printf("1) Slow Mode: Add a delay between scenes so your eyes can keep up with the actions.\n");
        printf("2) Fast Mode: Run the code with no delay and without it visually refreshing.\n");
        printf("press 'q' to exit\n");
    }
    c = getchar();
    switch (c) {
        case '1':
            printf("Running in slow mode: please enter delay duration (in milliseconds)\n");
            scanf("%d", &delay);
            info->delay = delay;
            break;
        case '2':
            printf("Running in fast mode\n");
            info->delay = 0;
            break;
        case 'q':
            printf("exiting...\n");
            return -1;
        default:
            i = true;
            return get_run_mode(info);
    }
    printf("please enter 1 to run the code on the CPU or 2 to run on the GPU\n");
    getchar();
    c = getchar();
    switch (c) {
        case '1':
            printf("Executing on the CPU\n");
            info->device = CPU;
            break;
        case '2':
            printf("Executing on the GPU\n");
            info->device = GPU;
            break;
        default:
            printf("Command not recognized: exiting...\n");
            return -1;
    }
    return 0;
}