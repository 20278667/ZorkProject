#ifndef ZORKLE_H
#define ZORKLE_H

#include "puzzle.h"
#include <iostream>
#include <fstream>
#include <random>

//inherits abstract class Puzzle
class Zorkle : public Puzzle {
public:
    Zorkle();
    unsigned int totalGuesses;
    void Reset();
    bool isCorrect();
    vector<string> outputState();
    void tryInput(string);
    string word;
    vector<string> words;
private:
    string formatLine(string, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
    string intToString(unsigned int);
    string randomWord();
    string toLower(string);
    bool validInput(string);
    bool containsWord(string);
};

#endif // ZORKLE_H
