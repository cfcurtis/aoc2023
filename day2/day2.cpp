#include <iostream>
#include <fstream>

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
    string word;
    int psum = 0, num;
    int game = -1;
    bool possible = false;

    while (in >> word) {
	if (word[word.length() - 1] == ':') {
	    // start of a new game
	    word.pop_back();
	    game = stoi(word);
	    possible = true;
	} else if (isdigit(word[0]))
	    num = stoi(word);
        else if (word.find("blue") != word.npos && num > 14) 
		possible = false;
	else if (word.find("red") != word.npos && num > 12)
		possible = false;
	else if (word.find("green") != word.npos && num > 13)
		possible = false;
        else if (word == "Game" && possible)
		psum += game;
    }
    // account for the last one
    if (possible)
	psum += game;
    
    return psum;
}

int part2(istream &in) {
    string word;
    int psum = 0, num;
    int blue = 0, red = 0, green = 0;

    while (in >> word) {
        if (isdigit(word[0]))
	    num = stoi(word);
        else if (word.find("blue") != word.npos) {
	    if (num > blue)
		blue = num;
	} 
	else if (word.find("red") != word.npos) {
	    if (num > red)
		red = num;
	}
	else if (word.find("green") != word.npos) {
	    if (num > green)
		green = num;
	}
        else if (word == "Game") {
	    // new game, reset everything
	    psum += red * green * blue;
	    red = 0, green = 0, blue = 0;
	    
	}
    }
    // account for the last one
    psum += red * green * blue;
    
    return psum;
}
