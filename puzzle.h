#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>
#include <vector>

using namespace std;

//pure virtual class
class Puzzle {
public:
    Puzzle() { }
    unsigned int remainingGuesses;
    virtual void Reset() = 0;
    virtual bool isCorrect() { return false; }
    virtual void tryInput(string) { };
    virtual vector<string> outputState() { return vector<string> {""}; }
};

#endif // PUZZLE_H
