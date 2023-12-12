#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

using namespace std;
typedef vector<vector<char> > Grid;

struct Node {
    int r;
    int c;
    int dist;
};

bool operator == (const Node &lhs, const Node &rhs) {
    return lhs.r == rhs.r && lhs.c == rhs.c;
}

bool operator != (const Node &lhs, const Node &rhs) {
    return !(lhs == rhs);
}

Grid read_grid(istream &in) {
    Grid grid;
    string line;
    while (in >> line) {
	vector<char> row(line.begin(), line.end());
	grid.push_back(row);
    }
    return grid;
}

Node find_start(const Grid &grid) {
    Node start = {0, 0, 0};
    for (int row = 0; row < grid.size(); ++row) {
	for (int col = 0; col < grid[row].size(); ++col) {
	    if (grid[row][col] == 'S') {
		start.r = row;
		start.c = col;
		return start;
	    }
	}
    }
    cout << "This should never happen\n";
    return start;
}

Node neighbour(const Grid &grid, Node &curr, Node &prev) {
    // regardless of the direction, distance is current +1
    Node n = {0, 0, curr.dist + 1};
    char dir;
    switch (grid[curr.r][curr.c]) {
    case '|':
	if (curr.r > prev.r)
	    dir = 'S';
	else
	    dir = 'N';
	break;
    case '-':
	if (curr.c > prev.c)
	    dir = 'E';
	else
	    dir = 'W';
	break;
    case 'L':
	if (curr.c == prev.c)
	    dir = 'E';
	else
	    dir = 'N';
	break;
    case 'J':
	if (curr.c == prev.c)
	    dir = 'W';
	else
	    dir = 'N';
	break;
    case '7':
	if (curr.c == prev.c)
	    dir = 'W';
	else
	    dir = 'S';
	break;
    case 'F':
	if (curr.c == prev.c)
	    dir = 'E';
	else
	    dir = 'S';
	break;
    case '.':
    case 'S':
    default: 
	cout << "Found a " << grid[curr.r][curr.c] << " at ["
	     << curr.r << ", " << curr.c << "]\n";
    }

    switch (dir) {
    case 'N':
	n.r = curr.r - 1;
	n.c = curr.c;
	break;
    case 'S':
	n.r = curr.r + 1;
	n.c = curr.c;
	break;
    case 'E':
	n.r = curr.r;
	n.c = curr.c + 1;
	break;
    case 'W':
	n.r = curr.r;
	n.c = curr.c - 1;
	break;
    }
    return n;
}

void find_first_adjacent(const Grid &grid, deque<Node> &dists) {
    Node s = dists.front();
    // special case for the first one
    Node n1 = {-1, -1, 1};
    Node n2 = {-1, -1, 1};
    string N = "|7F";
    string S = "|LJ";
    string E = "-J7";
    string W = "-LF";

    // look north
    if (s.r > 0 && N.find(grid[s.r-1][s.c]) != string::npos) {
	n1.r = s.r-1;
	n1.c = s.c;
    }
    // look south
    if (s.r < grid.size() - 1 && S.find(grid[s.r+1][s.c]) != string::npos) {
	Node *n = &n1;
	if (n->r != -1)
	    n = &n2;
	n->r = s.r+1;
	n->c = s.c;
    }
    // look west
    if (s.c > 0 && W.find(grid[s.r][s.c-1]) != string::npos) {
	Node *n = &n1;
	if (n->r != -1)
	    n = &n2;
	n->r = s.r;
	n->c = s.c-1;
    }
    // look east
    if (s.c < grid[0].size()-1 && E.find(grid[s.r][s.c+1]) != string::npos) {
	Node *n = &n1;
	if (n->r != -1)
	    n = &n2;
	n->r = s.r;
	n->c = s.c+1;
    }
    dists.push_front(n1);
    dists.push_back(n2);    
}

void find_adjacent(const Grid &grid, deque<Node> &dists) {
    if (dists.front() == dists.back())
	return;

    dists.push_back(neighbour(grid, dists.back(), *(dists.end() - 2)));
    dists.push_front(neighbour(grid, dists.front(), *(dists.begin() + 1)));
    find_adjacent(grid, dists);
}

int part1(istream &in) {
    Grid grid = read_grid(in);
    deque<Node> dists;
    Node s = find_start(grid);
    dists.push_front(s);
    find_first_adjacent(grid, dists);
    find_adjacent(grid, dists);

    int furthest = 0;
    for (deque<Node>::iterator it = dists.begin(); it != dists.end(); ++it) {
	if (it->dist > furthest)
	    furthest = it->dist;
    }
    
    return furthest;
}

int part2(istream &in) {
    return 0;
 }

int main(int argc, char *argv[]) {
    ifstream in(argv[1]);
    cout << "Part 1: " << part1(in) << endl;
    in.clear();
    in.seekg(0);
    cout << "Part 2: " << part2(in) << endl;
    in.close();
    return 0;
}
