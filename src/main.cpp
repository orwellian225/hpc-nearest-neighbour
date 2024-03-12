#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "VecN.hpp"

int main(int argc, char **argv) {

    size_t size = 3;
    if (argc == 2)
        size = atoi(argv[1]);
        
    srand(time(NULL));

    VecN a(size, 2.);
    VecN b(size, 2.);
    a.randomize(); b.randomize();
    a.print();
    b.print();

    printf("Norm L1(a,b): %f\n", a.norm1(b));
    printf("Norm L2(a,b): %f\n", a.norm2(b));


    return 0;
}
