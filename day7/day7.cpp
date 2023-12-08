#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

struct Hand {
    vector<int> cards;
    int bid;
    int score;
    bool operator < (const Hand &other) const;
};

bool Hand::operator < (const Hand &other) const {
    if (score != other.score)
	return score < other.score;
    else {
	for (int i = 0; i < 5; ++i) {
	    if (cards[i] != other.cards[i])
		return cards[i] < other.cards[i];
	}
    }
    return false; // equal at this point
}

void score_hand(Hand &hand, bool part1);
Hand read_hand(istream &in, bool part1);
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

Hand read_hand(istream &in, bool part1) {
    Hand hand;
    char c;
    for (int i = 0; i < 5; ++i) {
	in >> c;
	if (isdigit(c))
	    hand.cards.push_back(int(c - '0'));
	else {
	    switch (c) {
	    case 'T':
		hand.cards.push_back(10);
		break;
	    case 'J':
		if (part1)
		    hand.cards.push_back(11);
		else
		    hand.cards.push_back(0);
		break;
	    case 'Q':
		hand.cards.push_back(12);
		break;
	    case 'K':
		hand.cards.push_back(13);
		break;
	    case 'A':
		hand.cards.push_back(14);
		break;
	    default:
		cout << "Unrecognized card " << c << endl;
	    }
	}
    }
    in >> hand.bid;
    in >> ws;
    return hand;
}
	
void score_hand(Hand &hand, bool part1) {
    vector<int> cards = hand.cards;
    set<int> u_cards(cards.begin(), cards.end());
    
    if (!part1 && find(cards.begin(), cards.end(), 0) != cards.end()) {
	// temporarily replace jokers for the best card combo
	int highest_count = 0;
	int highest_card = -1;
	
	for (set<int>::iterator it = u_cards.begin(); it != u_cards.end(); ++it) {
	    int n_cards = count(cards.begin(), cards.end(), *it);
	    if (*it != 0 && n_cards > highest_count) {
		highest_count = n_cards;
		highest_card = *it;
	    }
	}	
	for (vector<int>::iterator it = cards.begin(); it != cards.end(); ++it) {
	    if (*it == 0)
		*it = highest_card;
	}
	u_cards.erase(0);
	if (u_cards.size() == 0) {
	    // only jokers
	    u_cards.insert(0);
	}
    }
    
    switch (u_cards.size()) {
    case 5:
    case 4:
	hand.score = 5 - u_cards.size();
	break;
    case 3: // either two pair or three of a kind
	hand.score = 2; // assume 2 pair
	for (set<int>::iterator it = u_cards.begin(); it != u_cards.end(); ++it)
	    if (count(cards.begin(), cards.end(), *it) == 3)
		hand.score = 3;
	break;
    case 2: // full house or four of a kind
	hand.score = 4; // assume full house
	for (set<int>::iterator it = u_cards.begin(); it != u_cards.end(); ++it)
	    if (count(cards.begin(), cards.end(), *it) == 4)
		hand.score = 5;
	break;	    
    case 1:
	hand.score = 6;
	break;
    default:
	cout << "Unexpected case hit! Number of cards: " << u_cards.size() << endl;
    }
}

int part1(istream &in) {
    vector<Hand> hands;

    while (!in.eof())
	hands.push_back(read_hand(in, true));

    for (int i = 0; i < hands.size(); ++i)
	score_hand(hands[i], true);

    int winnings = 0;
    sort(hands.begin(), hands.end());
    for (int i = 0; i < hands.size(); ++i)
	winnings += (i + 1) * hands[i].bid;
    
    return winnings;
}

int part2(istream &in) {
    
    vector<Hand> hands;

    while (!in.eof())
	hands.push_back(read_hand(in, false));

    for (int i = 0; i < hands.size(); ++i)
	score_hand(hands[i], false);

    int winnings = 0;
    sort(hands.begin(), hands.end());
    for (int i = 0; i < hands.size(); ++i)
	winnings += (i + 1) * hands[i].bid;
    
    return winnings;
 }
