#ifndef CHARACTER_H_
#define CHARACTER_H_
#include "ZorkUL.h"

#include <string>
#include <vector>

using namespace std;
using std::vector;


class Character {
private:
    string description;
public:
    void addItem(Item*);
    void addItem(Item);
    vector <Item*> inventory;

public:
	Character(string description);
    ~Character();
    string shortDescription();

};

#endif /*CHARACTER_H_*/
