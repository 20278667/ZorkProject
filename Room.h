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

public:
    Room(const Room*);
    Puzzle* puzzle = NULL;
    int numberOfItems();
    vector <Item> itemsInRoom;
    Room(string name, string description);
	void setExits(Room *north, Room *east, Room *south, Room *west);
	string shortDescription();
	string longDescription();
	Room* nextRoom(string direction);
    void addPuzzle(Puzzle*);
    bool hasPuzzle();
    void addItem(Item*);
    string displayItem();
    int isItemInRoom(string);
    void removeItemFromRoom(int location);
};

#endif
