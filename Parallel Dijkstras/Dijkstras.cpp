#include "Dijkstras.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <omp.h>
#include <climits>

using namespace std;

/*
 * Constructor to initialize graph and call necessary functions 
 */
Dijkstras::Dijkstras(int vertices): V(vertices){
	double start, end;
	start = omp_get_wtime();

	initInput();
	min_distance = dijkstra_distance();

	cout << "\nMinimum distances from node 0:\n";
	
	for (int i = 0; i < V; i++ ){
		cout << "  " << setw(2) << i << "  " << setw(2) << min_distance[i] << "\n";
	}
	
	delete [] min_distance;			//Free memory after program completes
	end = omp_get_wtime();
	cout << "\nTime taken: " << end - start << endl;
}

/*
 * This function calculates the minimum distance from source to every vertex using Dijkstra's minimum distance algorithm
 */
int* Dijkstras::dijkstra_distance(){

	bool *visited;
	int i, md, mv, my_first, my_last, my_id, my_md, my_mv, my_step, nth;
	int *min_distance;

	visited = new bool[V];
	visited[0] = true;					//Distance from source to itself will always be 0 i.e. the minimum
	for ( i = 1; i < V; i++ ){			//Initialzie all other vertices to false
		visited[i] = false;
	}

	min_distance = new int[V];

	for ( i = 0; i < V; i++ ){			//Set the direct distance from source to all other vertices as minimum
		min_distance[i] = graph[0][i];
	}

	//The private and shared directives specify the usage of every variable by the thread
	# pragma omp parallel private ( my_first, my_id, my_last, my_md, my_mv, my_step ) shared ( visited, md, min_distance, mv, nth, graph )
	{
	
		// This block divides the vertices equally among all the available threads
		my_id = omp_get_thread_num();
		nth = omp_get_num_threads(); 
		my_first =   (   my_id       * V ) / nth;
		my_last  =   ( ( my_id + 1 ) * V ) / nth - 1;
		
		//Using single directive to print the number of available threads only once
		# pragma omp single
		{
			cout << "\nParallel region begins with " << nth << " threads.\n\n";
		}

		//The loop runs V times to calculate minimum distance to the source
		for ( my_step = 1; my_step < V; my_step++ ){
	
			//Initialzie intermediate minimum distance and intermediate vertex for later comparison
			# pragma omp single 
			{
				md = inf;
				mv = -1; 
			}
	
			//Calculate the minimum distance parallely in n number of threads
			find_nearest ( my_first, my_last, min_distance, visited, &my_md, &my_mv );
	
			//Every thread should update the intermediate minimum distance and nearest vertex one at a time
			//Every thread compares with the intermediate minimum distance with other threads and decides which one is the nearest vertex
			# pragma omp critical
			{
				if ( my_md < md ){
				md = my_md;
				mv = my_mv;
				}
			}
	
			//Wait untill all the threads have calculated minimum distances and intermediate vertices
			# pragma omp barrier
	
			//Update the nearest found vertex as visited
			# pragma omp single 
			{
				if ( mv != -1 ){
					visited[mv] = true;

						cout << "Iteration:" << my_step << "\t"
							<< "Minimum distance:" << md << "\t"
							<< "Vertex:" << mv << endl;  

				}
			}
	
			//After all threads have reached this step, update the minimum distances for all the vertices
			# pragma omp barrier
			if ( mv != -1 ){
				update_mind( my_first, my_last, mv, visited, min_distance );
			}
		
			#pragma omp barrier
		}
	
		# pragma omp single
		{
		cout << "\nExiting parallel region.\n";
		}
	}

	//Delete when no more required
	delete [] visited;

	return min_distance;
}

/*
 * This function calculates the minimum distance for the specifies number of vertices between start and end
 * @param start vertex
 * @param end vertex
 * @param minimum distances from source
 * @param visited vertices
 * @param intermediate minimum distance
 * @param nearest vertex
 */
void Dijkstras::find_nearest( int start, int end, int min_distance[1000], bool visited[1000], int *d, int *v ){
	*d = inf;
	*v = -1;
	for (int i = start; i <= end; i++ ){
	if (!visited[i] && min_distance[i] < *d ){
		*d = min_distance[i];
		*v = i;
	}
	}
}

/*
 * This function updates the minimum distance for the specifies number of vertices between start and end
 * @param start vertex
 * @param end vertex
 * @param minimum distances from source
 * @param visited vertices
 * @param intermediate minimum distance
 * @param nearest vertex
 */
void Dijkstras::update_mind( int start, int end, int mv, bool visited[1000], int min_distance[1000]){
	for (int i = start; i <= end; i++ ){
	if (!visited[i] ){
		if (graph[mv][i] < inf ){
		if (min_distance[mv] + graph[mv][i] < min_distance[i] ){
			min_distance[i] = min_distance[mv] + graph[mv][i];
		}
		}
	}
	}
}

/*
 * Initialzie the input graph
 * Graph with 6 and 8 number of vertices are static graphs while graph with 1000 vertices is a dense randonly generated graph
 */
void Dijkstras::initInput(){
	for(int i = 0; i < V; i++ ){
		for(int j = 0; j < V; j++ ){
			if ( i == j ){
				graph[i][i] = 0;
			}
			else{
				graph[i][j] = inf;
			}
		}
	}
	
	if(V==6){
		graph[0][1] = graph[1][0] = 40;
		graph[0][2] = graph[2][0] = 15;
		graph[1][2] = graph[2][1] = 20;
		graph[1][3] = graph[3][1] = 10;
		graph[1][4] = graph[4][1] = 25;
		graph[2][3] = graph[3][2] = 100;
		graph[1][5] = graph[5][1] = 6;
		graph[4][5] = graph[5][4] = 8;	
	}else if(V==8){
		graph[0][1] = graph[1][0] = 8;
		graph[0][2] = graph[2][0] = 2;
		graph[0][3] = graph[3][0] = 5;
		graph[1][3] = graph[3][1] = 2;
		graph[2][3] = graph[3][2] = 2;
		graph[1][5] = graph[5][1] = 13;
		graph[3][4] = graph[4][3] = 1;
		graph[2][4] = graph[4][2] = 5;
		graph[3][5] = graph[5][3] = 6;
		graph[3][6] = graph[6][3] = 3;
		graph[4][6] = graph[6][4] = 1;
		graph[6][5] = graph[5][6] = 2;
		graph[7][5] = graph[5][7] = 3;
		graph[7][6] = graph[6][7] = 6;
	}else{
		srand(time(NULL));
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				if ( i != j ){
					graph[i][j] = graph[j][i] = j + rand() % 1000;
				}
			}
		}
		return;
	}
	print_graph();
}

/*
 * Print the initialized graph
 */
void Dijkstras::print_graph(){
	cout << "\nGraph:\n";
	for (int i = 0; i < V; i++ ){
		for (int j = 0; j < V; j++ ){
			if(graph[i][j] == inf){
				cout << "  " << setw(3) <<  "Inf";
			}else{
				cout << "  " << setw(3) <<  graph[i][j];
			}
		}
		cout << "\n";
	}
}