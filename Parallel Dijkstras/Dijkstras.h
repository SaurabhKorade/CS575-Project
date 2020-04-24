#include <cstring>
#include <string>
#include <climits>

using namespace std;

class Dijkstras
{
public: 
    explicit Dijkstras(int);
    
private:
	int graph[1000][1000], V;
	int *min_distance;

public:
	int inf = INT_MAX;
	
private:
	void initInput();
	void print_graph();
	void update_mind( int s, int e, int mv, bool connected[6], int mind[6]);
	void find_nearest( int s, int e, int mind[6], bool connected[6], int *d, int *v );
	int *dijkstra_distance();
};
