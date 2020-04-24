#include "BitonicSort.h"
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <omp.h>

using namespace std;

BitonicSort::BitonicSort(int elements, int threads): noOfElements(elements) ,noOfThreads(threads){
	omp_set_dynamic(0); 							// Disabled so that the os doesnt override the thread settings
	//int threadCount = omp_get_num_procs(); 			// Gives number of logical cores
	omp_set_num_threads(noOfThreads); 				
	
	//cout << "No. of threads:" << threadCount << endl;
	cout << "No of elements:" << noOfElements << endl;
	initInput();
	
	print_data();
	
	double start, end;
	start = omp_get_wtime();
	
	bitonicSort();
	
	end = omp_get_wtime();
	cout << "Sorted:";
	print_data();
	
	cout << "Time taken: " << end - start << endl;
}

void BitonicSort::initInput(){
	srand(time(NULL));
	for (int i = 0; i < noOfElements; i++) {
		this->input.push_back(i + rand() % 1000);
	}
}

void BitonicSort::conditionalSwap(int index1, int index2, bool condition){
	if(condition){
		if (input[index2] < input[index1]) {
			int temp = input[index2];
			input[index2] = input[index1];
			input[index1] = temp;
		}
	}else{
		if (input[index1] < input[index2]) {
			int temp = input[index2];
			input[index2] = input[index1];
			input[index1] = temp;
		}
	}
}

void BitonicSort::print_data(){
	for(int i=0; i<noOfElements; i++){
  		cout << input[i]<< "\t";
  	}
	cout<<endl;
}

void BitonicSort::bitonicMerge(int startIndex, int lastIndex, bool dir){
  if (dir){
    int counter = 0; 
    int elements = lastIndex - startIndex + 1;
    for (int j = elements / 2; j > 0; j = j / 2) {
      counter = 0;
      for (int i = startIndex; i + j <= lastIndex; i++){
        if (counter < j) {
			//cout << "i:" << i<< "\ti+j:" << i+j << endl;
          conditionalSwap(i, i + j,true);
          counter++;
        } else {
          counter = 0;
          i = i + j - 1;
        }
      }
    }
  } else{
    int counter = 0;
    int elements = lastIndex - startIndex + 1;
    for (int j = elements / 2; j > 0; j = j / 2) {
      counter = 0;
      for (int i = startIndex; i <= (lastIndex - j); i++){
        if (counter < j) {
			//cout << "i:" << i<< "\ti+j:" << i+j << endl;
          conditionalSwap(i, i + j,false);
          counter++;
        } else{
          counter = 0;
          i = i + j - 1;
        }
      }
    }
  }
}

void BitonicSort::bitonicSort(){
  for (int i = 2; i <= noOfElements; i *= 2) {
    #pragma omp parallel for 
    for (int j = 0; j < noOfElements; j = i + j) {
      if (((j / i) % 2) == 0) {
		  //cout << "startIndex:" << j << "\tlastIndex:" << i+j-1 << "\tDir=1\tData before sort:\n";
		  bitonicMerge(j, i + j - 1, true);
      } else {
		  //cout << "startIndex:" << i << "\tlastIndex:" << i+j-1 << "\tDir=0\tData before sort:\n";
		  bitonicMerge(j, i + j - 1, false);
      }
    }
  }
}