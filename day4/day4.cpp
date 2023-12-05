#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

set<int> read_set(istream &in);
void parse(istream &in, vector<set<int> > &winning, vector<set<int> > &on_card);
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

set<int> read_set(istream &in) {
    set<int> s;
    int temp;
    in >> ws;
    int c = in.peek();
    while (isdigit(c)) {
	in >> temp >> ws;
	s.insert(temp);
	c = in.peek();
    }
    char unused;
    in >> unused;
    return s;
}

void parse(istream &in, vector<set<int> > &winning, vector<set<int> > &on_card) {
    char card[5];

    int temp;
    char unused;
    // read Card ##:
    in >> card >> temp >> unused;
    while (!in.eof()) {
	winning.push_back(read_set(in));
	on_card.push_back(read_set(in));
    
	// Doesn't matter if we hit the end and have duplicate data
	in >> card >> temp >> unused;    
    }
}

int part1(istream &in) {
    vector<set<int> > winning;
    vector<set<int> > on_card;
    parse(in, winning, on_card);
    
    // check how many present are in winning
    int points = 0;
    vector<set<int> >::iterator win_it = winning.begin();
    vector<set<int> >::iterator card_it = on_card.begin();
    for (; win_it != winning.end() || card_it != on_card.end(); ++win_it, ++ card_it) {
	set<int> isect;
	set_intersection(win_it->begin(), win_it->end(),
			 card_it->begin(), card_it->end(),
			 inserter(isect, isect.begin()));
	int n_matches = isect.size();
	if (n_matches > 0)
	    points += pow(2, n_matches - 1);
    }
    return points;
}

int part2(istream &in) {
    vector<set<int> > winning;
    vector<set<int> > on_card;
    parse(in, winning, on_card);
    
    // check how many present are in winning
    vector<set<int> >::iterator win_it = winning.begin();
    vector<set<int> >::iterator card_it = on_card.begin();
    vector<int> wins;
    for (; win_it != winning.end() || card_it != on_card.end(); ++win_it, ++card_it) {
	set<int> isect;
	set_intersection(win_it->begin(), win_it->end(),
			 card_it->begin(), card_it->end(),
			 inserter(isect, isect.begin()));
	wins.push_back(isect.size());
    }

    // back to arrays
    int cards[256] = {};
    for (vector<int>::iterator it = wins.begin(); it != wins.end(); ++it) {
	int n_next = *it;
	int i = it - wins.begin();
	cards[i] += 1; // add on the original card
	int n_cards = cards[i]; // how many copies to add to the next ones?
	for (int j = i + 1; j <= i + n_next; ++j)
	    cards[j] += n_cards;
    }

    // add them up
    int sum = 0;
    for (int i = 0; i < wins.size(); ++i)
	sum += cards[i];
    
    return sum;
 }
