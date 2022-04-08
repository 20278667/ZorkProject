#include "Character.h"

Character::Character(string description) {
	this->description = description;
}

void Character::addItem(Item *item) {
    inventory.push_back(item);
}

void Character::addItem(Item item) {
    Item i = item;
    inventory.push_back(&i);
}

string Character::shortDescription() {
    return description;
}

