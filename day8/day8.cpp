#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include <sstream>

using namespace std;

map<string, pair<string, string> > read_map(istream &in);
long unsigned int lcm(long unsigned int n1, long unsigned int n2);
long unsigned int gcd(long unsigned int n1, long unsigned int n2);
int part1(istream &in);
long unsigned int part2(istream &in);

int main(int argc, char *argv[]) {
    ifstream in(argv[1]);
    cout << "Part 1: " << part1(in) << endl;
    in.clear();
    in.seekg(0);
    cout << "Part 2: " << part2(in) << endl;
    in.close();
    return 0;
}

map<string, pair<string, string> > read_map(istream &in) {
    map<string, pair<string, string> > nodes;
    string key;
    char temp;
    string l, r;
    while (in >> key) {
	in >> temp; // the = sign
	in >> l; // (AAA,
	in >> r; // BBB)
	pair<string, string> lr(l.substr(1,3), r.substr(0,3));
	nodes.insert(pair<string, pair<string, string> >(key, lr));
    }
    return nodes;
}

int part1(istream &in) {
    vector<char> dirs;
    char dir;

    while (in.peek() != '\n') {
	in >> dir;
	dirs.push_back(dir);
    }

    map<string, pair<string, string> > nodes = read_map(in);
    
    int steps = 0;
    string loc = "AAA";
    vector<char>::iterator dirit = dirs.begin();
    
    while (loc != "ZZZ") {
	++steps;
	if (*dirit == 'L')
	    loc = nodes[loc].first;
	else
	    loc = nodes[loc].second;
	
	++dirit;
	if (dirit == dirs.end())
	    dirit = dirs.begin();
    }
    
    return steps;
}

long unsigned int gcd(long unsigned int n1, long unsigned int n2) {
    long unsigned int max = n1;
    long unsigned int min = n2;
    if (n2 > n1) {
	max = n2;
	min = n1;
    }

    while (min != 0) {
	long unsigned int temp = min;
	min = max % min;
	max = temp;
    }
    
    return max;
}

long unsigned int lcm(long unsigned int n1, long unsigned int n2) {
    return n1 * n2 / gcd(n1, n2);
}

long unsigned int part2(istream &in) {
    vector<char> dirs;
    char dir;

    while (in.peek() != '\n') {
	in >> dir;
	dirs.push_back(dir);
    }
    
    map<string, pair<string, string> > nodes = read_map(in);

    vector<string> locs;
    // find all the spots ending in A
    for (map<string, pair<string, string> >::iterator it = nodes.begin();
	 it != nodes.end(); ++it) {
	if (it->first[2] == 'A')
	    locs.push_back(it->first);
    }

    vector<long unsigned int> steps(locs.size(), 0);
    vector<char>::iterator dirit = dirs.begin();

    for (int i = 0; i < locs.size(); ++i) {
	string loc = locs[i];
	while (loc[2] != 'Z') {
	    ++steps[i];
	    if (*dirit == 'L')
		loc = nodes[loc].first;
	    else
		loc = nodes[loc].second;
	
	    ++dirit;
	    if (dirit == dirs.end())
		dirit = dirs.begin();
	}
    }

    // I cheated on the lcm part, originally tried to brute force and it
    // was taking forever so I checked subreddit which said lcm works.
    // Still not really convinced that it necessarily should.
    while (steps.size() > 1) {
        steps[0] = lcm(steps[0], steps[1]);
	steps.erase(steps.begin() + 1);
    }
    
    return steps[0];
 }
