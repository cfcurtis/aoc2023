#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

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

int part1(istream &in) {
    return 0;
}

int part2(istream &in) {
    return 0;
 }
