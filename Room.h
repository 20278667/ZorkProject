#ifndef ROOM_H_
#define ROOM_H_

#include <map>
#include <string>
#include <vector>
#include "item.h"
#include "puzzle.h"
using namespace std;
using std::vector;

class Room {

private:
    string name;
	string description;
	map<string, Room*> exits;
	string exitString();
    vector <Item> itemsInRoom;
    Puzzle* p;

public:
    int numberOfItems();
    Room(string name, string description);
	void setExits(Room *north, Room *east, Room *south, Room *west);
	string shortDescription();
	string longDescription();
	Room* nextRoom(string direction);
    void addPuzzle(Puzzle*);
    bool hasPuzzle();
    void addItem(Item *inItem);
    string displayItem();
    int isItemInRoom(string inString);
    void removeItemFromRoom(int location);
};

#endif
