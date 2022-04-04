#ifndef ZORKLE_H
#define ZORKLE_H

#include <vector>
#include "puzzle.h"

//inherits abstract class Puzzle
class Zorkle : public Puzzle {
public:
    Zorkle();
    void Reset();
    bool isCorrect();
    void tryInput();
    string outputState();
    string word;
    vector<string> words;
private:
    int totalGuesses;
    int remainingGuesses;
    string randomWord();
};

#endif // ZORKLE_H
