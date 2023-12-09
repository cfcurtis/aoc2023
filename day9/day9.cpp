#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

template<typename T>
void print_vec(vector<T> vec) {
    for (typename vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
	cout << *it << ' ';
    cout << endl;
}

bool all(vector<int> vals, int num) {
    for (vector<int>::iterator it = vals.begin(); it != vals.end(); ++it) {
	if (*it != num)
	    return false;
    }
    return true;
}

int extrapolate(vector<int> vals) {
    if (all(vals, 0))
	return 0;

    vector<int> diffs(vals.size() - 1, 0);
    for (int i = 0; i < diffs.size(); ++i)
	diffs[i] = vals[i+1] - vals[i];

    return vals.back() + extrapolate(diffs);
}

int extrapolate_backwards(vector<int> vals) {
    if (all(vals, 0))
	return 0;

    vector<int> diffs(vals.size() - 1, 0);
    for (int i = 0; i < diffs.size(); ++i)
	diffs[i] = vals[i+1] - vals[i];

    return vals.front() - extrapolate_backwards(diffs);
}

vector<vector<int >> parse(istream &in) {
    vector<vector<int> > nums;
    string line;
    int num;

    while (getline(in, line)) {
	istringstream ss(line);
	vector<int> oneline;
	while (ss >> num)
	    oneline.push_back(num);
	nums.push_back(oneline);
    }
    return nums;
}

int part1(istream &in) {
    vector<vector<int> > nums = parse(in);
    int psum = 0;
    for (int i = 0; i < nums.size(); ++i) {
	psum += extrapolate(nums[i]);
    }
    
    return psum;
}

int part2(istream &in) {
    vector<vector<int> > nums = parse(in);

    int psum = 0;
    for (int i = 0; i < nums.size(); ++i) {
	psum += extrapolate_backwards(nums[i]);
    }
    
    return psum;
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
