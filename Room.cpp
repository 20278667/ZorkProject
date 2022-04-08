#include "Room.h"
#include "Command.h"


Room::Room(string name, string description) {
    this->name = name;
	this->description = description;
}

//shallow copy constructor
Room::Room(const Room* r) {
    name = r->name;
    description = r->description;
    exits = r->exits;
    puzzle = r->puzzle;
    itemsInRoom = r->itemsInRoom;
}

void Room::setExits(Room *north, Room *east, Room *south, Room *west) {
	if (north != NULL)
		exits["north"] = north;
	if (east != NULL)
		exits["east"] = east;
	if (south != NULL)
		exits["south"] = south;
	if (west != NULL)
		exits["west"] = west;
}

void Room::addPuzzle(Puzzle *i) {
    puzzle = i;
}

bool Room::hasPuzzle() {
    return (puzzle != NULL);
}

string Room::shortDescription() {
	return description;
}

string Room::longDescription() {
    return name + "\n" + description + "\n" + displayItem() + exitString();
}

string Room::exitString() {
	string returnString = "\nexits =";
	for (map<string, Room*>::iterator i = exits.begin(); i != exits.end(); i++)
		// Loop through map
		returnString += "  " + i->first;	// access the "first" element of the pair (direction as a string)
	return returnString;
}

Room* Room::nextRoom(string direction) {
	map<string, Room*>::iterator next = exits.find(direction); //returns an iterator for the "pair"
	if (next == exits.end())
		return NULL; // if exits.end() was returned, there's no room in that direction.
	return next->second; // If there is a room, remove the "second" (Room*)
				// part of the "pair" (<string, Room*>) and return it.
}

void Room::addItem(Item *inItem) {
    itemsInRoom.push_back(*inItem);
}

void Room::removeItemFromRoom(int location) {
    itemsInRoom.at(location).setHidden(true);
}

string Room::displayItem() {
    string tempString = "Items in room: ";
    int sizeItems = (itemsInRoom.size());
    if (itemsInRoom.size() < 1) {
        tempString = "";
        }
    else if (itemsInRoom.size() > 0) {
        int x = (0);
        for (int n = sizeItems; n > 0; n--) {
            if (!itemsInRoom[x].getHidden()) {
                tempString = tempString + itemsInRoom[x].getShortDescription() + ", " ;
            }
            x++;
        }
    }
    return tempString;
}

int Room::numberOfItems() {
    return itemsInRoom.size();
}

int Room::isItemInRoom(string desc)
{
    if (itemsInRoom.size() == 0) return 0;
    else {
        for (unsigned int i = 0; i < itemsInRoom.size(); i++) {
            if (!itemsInRoom.at(i).getHidden() &&
                    desc.compare(itemsInRoom.at(i).getShortDescription()) == 0) return i;
        }
    }
    return -1;
}

