#include "sdl_grid.h"

int get_run_mode() {
    static bool i = false;
    char c;
    int delay;

    if (!i) {
        printf("Welcome to Conway's Game of Life\nPlease select an operating mode\n");
        printf("1) Slow Mode: Add a delay between scenes so your eyes can keep up with the actions\n");
        printf("2) Fast Mode: Run the code with no delay. It will run twice, once on the gpu and once on the cpu where the performance times will be compared\n");
        printf("press 'q' to exit\n");
    }
    c = getchar();
    switch (c) {
        case '1':
            printf("Running in slow mode: please enter delay duration (in milliseconds)\n");
            scanf("%d", &delay);
            return delay;
        case '2':
            printf("Running in fast mode\n");
            return 0;
        case 'q':
            printf("exiting...\n");
            return -1;
        default:
            i = true;
            return get_run_mode();
    }
}