#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

template<typename T>
void print_vec(vector<T> vec) {
    for (typename vector<T>::iterator it = vec.begin(); it != vec.end(); ++it)
	cout << *it << ' ';
    cout << endl;
}

int part1(istream &in) {
    return 0;
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
