#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct NumInfo {
    int val;
    int c1;
    int c2;
    int r;
};

const int N = 140 * 140; // way bigger than it needs to be

void parse(istream &in, NumInfo nums[], int &n_nums, int sym_coords[N][2], int &n_syms, bool p1);
bool adjacent(const NumInfo &num, int coord[2]);

int part1(istream &in);
int part2(istream &in);

int main(int argc, char *argv[]) {
    ifstream in(argv[1]);
    cout << "Part 1: " << part1(in) << endl;
    in.clear();
    in.seekg(0);
    cout << "Part 2: " << part2(in) << endl;
    in.close();
    return 0;
}

void parse(istream &in, NumInfo nums[], int &n_nums, int sym_coords[N][2], int &n_syms, bool p1) {
    streamoff row_start = 0;
    int row = 0;
    char unused;

    int pc = in.peek();
    while (!in.eof()) {
	while (pc != '\n') {
	    if (isdigit(pc)) {
		nums[n_nums].c1 = in.tellg() - row_start;
		in >> nums[n_nums].val;
		nums[n_nums].c2 = in.tellg() - row_start;
		nums[n_nums].r = row;
		++n_nums;
	    } else {
		// consume the character if it's not a number
		in >> unused;
	        if ((p1 && pc != '.') || (!p1 && pc == '*')) {
		    // hacky way to handle p1 and p2
		    sym_coords[n_syms][0] = row;
		    sym_coords[n_syms][1] = in.tellg() - row_start - streamoff(1);
		    ++n_syms;
		}
	    }
	    pc = in.peek();
	}
	// advance past the newline
	in.seekg(1, ios::cur);
	++row;
       	row_start = in.tellg();
	pc = in.peek();
    }

}


bool adjacent(const NumInfo &num, int coord[2]) {
    for (int c = num.c1; c < num.c2; ++c) {
	if (abs(coord[0] - num.r) <= 1 && abs(coord[1] - c) <= 1)
	    return true;
    }
    return false;
}

int part1(istream &in) {
    NumInfo nums[N];
    int sym_coords[N][2];
    
    int n_nums = 0, n_syms = 0;
    parse(in, nums, n_nums, sym_coords, n_syms, true);
    
    // now loop through the numbers and check for adjacency
    int sum = 0;
    for (int n = 0; n < n_nums; ++n) {
	for (int s = 0; s < n_syms; ++s) {
	    if (adjacent(nums[n], sym_coords[s])) {
		sum += nums[n].val;
		break;
	    }
	}
    }
    
    return sum;
}

int part2(istream &in) {
    NumInfo nums[N];
    int sym_coords[N][2];
    int n_nums = 0, n_syms = 0;
    parse(in, nums, n_nums, sym_coords, n_syms, false);
    
    // now loop through the gears and check for adjacency
    int sum = 0;
    for (int s = 0; s < n_syms; ++s) {
	int g1 = -1;
	for (int n = 0; n < n_nums; ++n) {
	    if (adjacent(nums[n], sym_coords[s])) {
		if (g1 == -1)
		    g1 = nums[n].val;
		else
		    sum += g1 * nums[n].val;
	    }
	}
    }
    
    return sum;

 }
