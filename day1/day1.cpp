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
    int sum = 0;
    const int N = 256;
    char line[N];

    in.getline(line, N);
    while (!in.eof()) {
	bool first = false;
	int second = 0;
	
	for (int i = 0; line[i] != '\0'; ++i) {
	    if (isdigit(line[i]) && !first) {
		second = int(line[i] - '0');
		sum += second * 10;
		first = true;
	    } else if (isdigit(line[i])) {
		second = int(line[i] - '0');
	    }
	}
	sum += second;
	in.getline(line, N);
    }
    return sum;
}

int part2(istream &in) {
    char numbers[][10] = {"zero","one","two","three","four",
			  "five","six","seven","eight","nine"};
    
    int sum = 0;
    const int N = 256;
    char line[N];

    while (in.getline(line, N)) {
	int pos[N];
	int vals[N];
	int n_vals = 0;
	
	// Loop through the word numbers to see if those exist
	for (int n = 0; n < 10; ++n) {
	    char *numstr = strstr(line, numbers[n]);
	    int offset = 0;
	    while (numstr) {
		pos[n_vals] = static_cast<int>(numstr - line + offset);
		vals[n_vals] = n;
		++n_vals;

		numstr = strstr(numstr + (++offset), numbers[n]);
	    }
	}

	// then check for digits
	for (int i = 0; line[i] != '\0'; ++i) {
	    if (isdigit(line[i])) {
		pos[n_vals] = i;
		vals[n_vals] = int(line[i] - '0');
		++n_vals;
	    }
	}

	int d1 = 0;
	int d2 = 0;
	int p1 = N;
	int p2 = -1;

	for (int i = 0; i < n_vals; ++i) {
	    if (pos[i] < p1) {
		d1 = vals[i];
		p1 = pos[i];
	    }

	    if (pos[i] > p2) {
		d2 = vals[i];
		p2 = pos[i];
	    }
	}
	
	// cout << d1 << d2 << endl;
	sum += d1 * 10 + d2;
    }
    return sum;
 }
