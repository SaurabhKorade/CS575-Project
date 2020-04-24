#include "BitonicSort.h"
#include <iostream>
#include <string>
#include <stdlib.h>
int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: parallel-bitonic <no_of_elements> <no_of_threads>" << std::endl;
        exit(1);
    }
	BitonicSort sort(atoi(argv[1]),atoi(argv[2]));
}
