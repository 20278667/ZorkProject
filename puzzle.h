#ifndef PUZZLE_H
#define PUZZLE_H

#include <string>

using namespace std;

//pure virtual class
class Puzzle {
public:
    Puzzle();
    virtual void Reset() = 0;
    virtual bool isCorrect();
    virtual void tryInput();
    virtual string outputState();
};

#endif // PUZZLE_H
