#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int start=0, stop, step=1, i;

    if(argc == 1) {
        printf("Usage: %s [start] stop [step]\n", argv[0]);
        return 0;
    }
    if (argc > 4) {
        fprintf(stderr, "Too many arguments\n");
        return 1;
    }

    if(argc == 2)
        stop = atoi(argv[1]);
    else {
        start = atoi(argv[1]);
        stop = atoi(argv[2]);
    }
    if (argc == 4) step=atoi(argv[3]);

    if(step == 0) {
        fprintf(stderr, "Step must not be zero\n");
        return 1;
    }

    for(i=start; step > 0 ? i < stop : i > stop; i+=step)
        printf("%d\n", i);
    return 0;
}
