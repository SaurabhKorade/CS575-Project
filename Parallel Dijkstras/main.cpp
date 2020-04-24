#include "Dijkstras.h"
#include <iostream>
#include <string>

int main(int argc, char **argv){
	
	//Check for valid number of arguments
	if (argc != 2) {
        std::cerr << "Usage: "<< argv[0] << " <vertices of graph>" << std::endl;
        exit(1);
    }
	
	int vertices = atoi(argv[1]);
	if(vertices == 6 || vertices == 8 || vertices == 1000){
		Dijkstras dijk(vertices);
	}else{
		std::cerr << "Number of vertices supported 6,8 or 1000." << std::endl;
	}
	
	
}
