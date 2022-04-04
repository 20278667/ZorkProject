#include "zorkle.h"

Zorkle::Zorkle() {

}

void Zorkle::Reset() {
    string word = randomWord();
    totalGuesses = 6;
    remainingGuesses = totalGuesses;
}

string randomWord() {
    return "";
}
