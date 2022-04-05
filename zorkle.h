#ifndef ZORKLE_H
#define ZORKLE_H

#include "puzzle.h"

//inherits abstract class Puzzle
class Zorkle : public Puzzle {
public:
    Zorkle();
    void Reset();
    bool isCorrect();
    vector<string> outputState();
    void tryInput(string);
    string word;
    vector<string> words;
private:
    int totalGuesses;
    int remainingGuesses;
    string formatLine(string, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
    string intToString(unsigned int);
    string randomWord();
    string toLower(string);
    bool validInput(string);
};

#endif // ZORKLE_H
