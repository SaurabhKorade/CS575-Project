#include <assert.h>
#include <cstring>
#include <string>
#include <vector>

using namespace std;
class BitonicSort
{
public: 
    explicit BitonicSort(int,int);
    //~BitonicSort();
    
private:
	void initInput();
	void print_data();
	void bitonicSort();
	void bitonicMerge(int ,int ,bool);
	void conditionalSwap(int , int , bool );

private:
    vector<int> input;
	int noOfElements,noOfThreads;
};
