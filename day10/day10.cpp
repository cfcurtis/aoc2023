#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>

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
    if (dists.front() == dists.back()) {
	dists.pop_back();
	return;
    }

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

int calc_dir(const Node &prev, const Node &curr, const Node &next, int uvec[2]) {
    // update the unit vector
    uvec[0] = next.r - curr.r;
    uvec[1] = next.c - curr.c;

    // straight ahead: one of them doesn't change
    if (curr.r == prev.r && curr.r == next.r ||
	curr.c == prev.c && curr.c == next.c) {
	return 0;
    }
    
    int dr = next.r - prev.r;
    int dc = next.c - prev.c;

    // left
    if (dr == dc && curr.r == prev.r ||
	dr != dc && curr.c == prev.c)
	return -1;
    else
	// right is the only thing left, right?
	return 1;

    // This took way too long to figure out
    //   0 1 2
    // 0 S---7
    // 1 |   |
    // 2 L---J
    //
    // Clockwise:    
    // (1,0) to (0,1) -> (-1,1) -> first r, then c
    // (0,1) to (1,2) -> (1,1) -> first c, then r
    // (1,2) to (2,1) -> (1,-1) -> first r, then c
    // (2,1) to (1,0) -> (-1,-1) -> first c, then r
    //
    // Counterclockwise
    // (0,1) to (1,0) -> (1,-1) -> first c, then r
    // (1,0) to (2,1) -> (1,1) -> first r, then c
    // (2,1) to (1,2) -> (-1,1) -> first c, then r
    // (1,2) to (0,1) -> (-1,-1) -> first r, then c
}

void r_flood_fill(Grid &grid, int r, int c) {
    // recursively flood fill until we run out of things to change
    if (r == 0 || c == 0 || r == (grid.size()-1) || c == (grid[0].size()-1) ||
	grid[r][c] == 'P' || grid[r][c] == '0')
	return; // don't fill here
    else {
	grid[r][c] = '0'; // fill with 0 just for fun
	r_flood_fill(grid, r-1, c-1);
	r_flood_fill(grid, r-1, c+1);
	r_flood_fill(grid, r+1, c-1);
	r_flood_fill(grid, r+1, c+1);
    }
}

void flood_fill(Grid &grid, int r, int c, int uvec[2], int turn) {
    // we need the pixel that's orthogonal to the unit vector
    // in the direction of the turn
    int ortho[2] = {turn * -uvec[1], turn * uvec[0]};
    r += ortho[0];
    c += ortho[1];
    r_flood_fill(grid, r, c);
}

int part2(istream &in) {
    Grid grid = read_grid(in);
    deque<Node> dists;
    Node s = find_start(grid);
    dists.push_front(s);
    find_first_adjacent(grid, dists);
    find_adjacent(grid, dists);
   
    // replace the real pipe with a P
    deque<Node>::iterator it;
    for (it = dists.begin(); it != dists.end(); ++it)
	grid[it->r][it->c] = 'P';
    
    // Now start walking the path, keeping track of turns
    int uvec[2];
    int turn = 0, last_turn = 0;
    
    for (it = dists.begin(); it < dists.end() - 1; ++it) {
	if (it == dists.begin())
	    turn = calc_dir(dists.back(), *it, *(it+1), uvec);
	else
	    turn = calc_dir(*(it-1), *it, *(it+1), uvec);
	
	if (turn != 0)
	    last_turn = turn;
	
	if (turn == 0 && last_turn != 0) {
	    // start looking towards the "inside" of the turn
	    flood_fill(grid, it->r, it->c, uvec, last_turn);
	}
    }

    // finally, count the zeros
    int filled = 0;
    for (Grid::iterator git = grid.begin(); git != grid.end(); ++git) {
	filled += count(git->begin(), git->end(), '0');
    }
    
    return filled;
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
