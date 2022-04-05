#include "zorkle.h"
#include <iostream>
#include <fstream>

const int wordCount = 2315;

Zorkle::Zorkle() {
    Reset();
    cout << word << endl;
}

void Zorkle::Reset() {
    string word = randomWord();
    totalGuesses = 6;
    remainingGuesses = totalGuesses;
    words.clear();
}

bool Zorkle::isCorrect() {
    bool correct = false;
    if (words.size() > 0) {
        for (unsigned int i = 0; i < words.size(); i++) {
            if (words[i].compare(word) == 0) {
                correct = true;
                break;
            }
        }
    }
    return correct;
}

void Zorkle::tryInput(string s) {
    if (!isCorrect()) {
        s = toLower(s);
        if (validInput(s)) {
            if (remainingGuesses != 0) {
                words.push_back(s);
                remainingGuesses--;
            }
        }
    }
}

vector<string> Zorkle::outputState() {
    vector<string> output;
    //bit structures used to satisfy submission criteria
    //2 bits needed for 3 possibilities: 00 => not present, 01 => present, incorrect place, 10 => present, correct place
    //11 => unecessary possibility.
    struct wordRanking {
        unsigned char a : 2;
        unsigned char b : 2;
        unsigned char c : 2;
        unsigned char d : 2;
        unsigned char e : 2;
    };
    //5 bits needed per letter of the word ([0,25] => [a,z]) => 7 wasted "extra letters"
    struct wordContent {
        unsigned char a : 5;
        unsigned char b : 5;
        unsigned char c : 5;
        unsigned char d : 5;
        unsigned char e : 5;
    } target, input;
    target.a = word[0] - 'a';
    target.b = word[1] - 'a';
    target.c = word[2] - 'a';
    target.d = word[3] - 'a';
    target.e = word[4] - 'a';

    if (words.size() > 0) {
        for (unsigned int i = 0; i < words.size(); i++) {
            //assign each word to wordContent input
            input.a = words[i][0] - 'a';
            input.b = words[i][1] - 'a';
            input.c = words[i][2] - 'a';
            input.d = words[i][3] - 'a';
            input.e = words[i][4] - 'a';
            //then compare with wordContent target, storing results in new wordRanking
            //warning: awful mess of ifs ahead
            wordRanking ranking;
            if (input.a == target.a) ranking.a = 2;
            else if (word.find(words[i][0]) != string::npos) ranking.a = 1;
            else ranking.a = 0;
            if (input.b == target.b) ranking.b = 2;
            else if (word.find(words[i][1]) != string::npos) ranking.b = 1;
            else ranking.b = 0;
            if (input.c == target.c) ranking.c = 2;
            else if (word.find(words[i][2]) != string::npos) ranking.c = 1;
            else ranking.c = 0;
            if (input.d == target.d) ranking.d = 2;
            else if (word.find(words[i][3]) != string::npos) ranking.d = 1;
            else ranking.d = 0;
            if (input.e == target.e) ranking.e = 2;
            else if (word.find(words[i][4]) != string::npos) ranking.e = 1;
            else ranking.e = 0;
            output.push_back(formatLine(words[i], ranking.a, ranking.b, ranking.c, ranking.d, ranking.e));
        }
    }
    return output;
}

inline string Zorkle::formatLine(string inWord, unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e) {
    return string(
            "\t<font color=\"#" + intToString(a) + "\">" + string(1, inWord[0]) + "</font>" +
            "\t<font color=\"#" + intToString(b) + "\">" + string(1, inWord[1]) + "</font>" +
            "\t<font color=\"#" + intToString(c) + "\">" + string(1, inWord[2]) + "</font>" +
            "\t<font color=\"#" + intToString(d) + "\">" + string(1, inWord[3]) + "</font>" +
            "\t<font color=\"#" + intToString(e) + "\">" + string(1, inWord[4]) + "</font>"
            );
}

inline string Zorkle::intToString(unsigned int input) {
    //{0,1,else} => {dark grey, yellow, green}
    if (input == 0) return "a9a9a9";
    else if (input == 1) return "ffe100";
    else return "07d92a";
    //<font color=\"#ff9900\">[sic]</font>
}


string Zorkle::randomWord() {
    srand (time(NULL));
    int wordLine = 1 + (rand() % wordCount);
    ifstream wordList("words.txt");
    //C:/Users/Perill/Documents/Qt/ZorkProject/
    string line;
    int count = 0;
    while (getline(wordList, line)) {
        count++;
        if (count == wordLine) {
            break;
        }
    }
    wordList.close();
    //exception
    return line;
}

bool Zorkle::validInput(string s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        if (s[i] < 'a' || s[i] > 'z') {
            return false;
        }
    }
    return true;
}

string Zorkle::toLower(string s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        if (isupper(s[i])) s[i] = tolower(s[i]);
    }
    return s;
}
