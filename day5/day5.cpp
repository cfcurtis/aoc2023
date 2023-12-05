#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct Range {
    long unsigned int start;
    long unsigned int end;
    long unsigned int offset;
};

vector<Range> read_map(istream &in);
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

long unsigned int transform(long unsigned int value, vector<Range> map) {
    for (vector<Range>::iterator it = map.begin(); it != map.end(); ++it) {
        if (value >= it->start && value < it->end)
            return value + it->offset;
    }
    // if it's not in the map, just pass it through
    return value;
}

vector<Range> read_map(istream &in) {
    // discard the header
    in >> ws;
    string unused;
    getline(in, unused);
    // cout << "Reading " << unused << endl;

    vector<Range> map;
    while (isdigit(in.peek())) {
        long unsigned int dest, source, length;
        in >> dest >> source >> length;
        Range r = {source, source + length, dest - source};
        map.push_back(r);
        in >> ws;
    }
    return map;
}

long unsigned int part1(istream &in) {
    // read the seeds
    string unused;
    in >> unused;

    vector<long unsigned int> seeds;
    vector<vector<Range>> maps;

    in >> ws;
    while (isdigit(in.peek())) {
        long unsigned int seed;
        in >> seed;
        seeds.push_back(seed);
        in >> ws;
    }

    // read the various maps
    while (!in.eof()) {
        maps.push_back(read_map(in));
        in >> ws;
    }

    // map the seeds
    vector<long unsigned int> locations = seeds;
    for (vector<vector<Range>>::iterator map_it = maps.begin();
         map_it != maps.end(); ++map_it) {
        for (vector<long unsigned int>::iterator l_it = locations.begin();
             l_it != locations.end(); ++l_it) {
            *l_it = transform(*l_it, *map_it);
        }
    }

    return *min_element(locations.begin(), locations.end());
}

long unsigned int part2(istream &in) {
    // read the seed ranges
    string unused;
    in >> unused;

    vector<Range> seeds;

    in >> ws;
    while (isdigit(in.peek())) {
        long unsigned int start;
        long unsigned int length;
        in >> start >> length;
        Range r = {start, start + length, 0};
        seeds.push_back(r);
        in >> ws;
    }
    
    // and the maps as before
    vector<vector<Range>> maps;
    while (!in.eof()) {
        maps.push_back(read_map(in));
        in >> ws;
    }

    // find the minimum for each range this time

    vector<long unsigned int> locations;

    for (vector<Range>::iterator s_it = seeds.begin(); s_it != seeds.end();
         ++s_it) {
        long unsigned int min_loc = ULONG_MAX;
        // for every value in the seed range, map it
        for (long unsigned int s = s_it->start; s < s_it->end; ++s) {
	    // transform with all the maps
	    long unsigned int loc = s;
            for (vector<vector<Range>>::iterator map_it = maps.begin();
                 map_it != maps.end(); ++map_it) {
                loc = transform(loc, *map_it);
            }
            if (loc < min_loc)
                min_loc = loc;
        }
        locations.push_back(min_loc);
    }

    return *min_element(locations.begin(), locations.end());
}
