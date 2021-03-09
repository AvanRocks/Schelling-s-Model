#include <vector>
#include <set>

enum Agent { X, O, EMPTY };

class Schelling {
private:
	int dy[8] = { 0, -1, -1, -1, 0, 1, 1, 1};
	int dx[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
	std::vector<std::vector<Agent>> grid;
	int threshold, sideSize;
	std::vector<std::pair<int,int>> dissatisfied, vacant;
	std::set<std::pair<int,int>> check;
	void getNeighbors(int y, int x, std::vector<Agent>& ret);
	void getNeighborsPos(int y, int x, std::vector<std::pair<int,int>>& ret);
	void move(int y, int x);

public:
	Schelling(int threshold, int xPercent, int emptyPercent, int size);
	void run();
	void outputGrid();
};
