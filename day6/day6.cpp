#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

using namespace std;

vector<double> readline(istream &in);
double readp2(istream &in);
long unsigned int n_ways(double time, double dist);
long unsigned int part1(istream &in);
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

vector<double> readline(istream &in) {
    vector<double> nums;
    string unused;
    string line;
    int num;

    getline(in, line);
    istringstream ss(line);
    ss >> unused;
    while (ss >> num)
	nums.push_back(num);
    
    return nums;
}

long unsigned int n_ways(double time, double dist) {
    double root_part = sqrt(time * time - (4 * (dist)));
    long unsigned int start = ceil((time - root_part) / 2 + 1e-6);
    long unsigned int end = floor((time + root_part) / 2 - 1e-6);
    return (end - start + 1);
}

long unsigned int part1(istream &in) {
    vector<double> time = readline(in);
    vector<double> dist = readline(in);

    long unsigned int ways = 1;
    for (size_t i = 0; i < time.size(); ++i) {
	ways *= n_ways(time[i], dist[i]);
   }
    
    return ways;
}

double readp2(istream &in) {
    string line;
    getline(in, line);
    istringstream ss(line);
    string temp;
    string nums;
    ss >> temp;
    while (ss >> temp)
	nums += temp;
    return atof(nums.c_str());
}
    
long unsigned int part2(istream &in) {
    double time = readp2(in);
    double dist = readp2(in);
    return n_ways(time, dist);
 }
