#include "ZorkUL.h"
#include "Character.h"

#define ROOMCOUNT 10

//data
MainWindow *w;
QApplication *a;
Character *player;
Room* rooms[ROOMCOUNT];

//flags
bool input = false;

//overloaded friend inline operator << for ease of sending data to MainWindow w for display.
//if statement seperates out tags from the body.
inline MainWindow* operator<< (MainWindow *m, QString s) {
    m->output(s);
    return m;
}

int main(int argc, char *argv[]) {
    a = new QApplication(argc, argv);
    w = new MainWindow();
    w->show();
    ZorkUL temp;
    thread t1(&ZorkUL::play, temp);
    int i = a->exec();
    delete w;
    delete a;
    t1.join();
    return i;
}

//constructor
ZorkUL::ZorkUL() {
    createRooms();
}

//main game loop
void ZorkUL::play() {
    while(!w->finished) {
        if (w->canPlay) {
            w->canPlay = false;
                if (w->input) {
                    w->input = false;
                    // Create pointer to command and give it a command.
                    Command* command = parser.getCommand(w->latestInput);
                    w->finished = processCommand(*command);
                    // Free the memory allocated by "parser.getCommand()" with ("return new Command(...)")
                    delete command;
                }
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    w << QString("End.");
    a->exit();
}


void ZorkUL::createRooms()  {
    //character creation
    player = new Character("A human being, of normal proportions and ability.");

    rooms[0] = new Room("Dungeon Cell", "A dank and dusty dungeon. Every surface is covered with a thick layer of dirt and grime.");
        rooms[0]->addItem(new Item("shovel", 100, 10));
    rooms[1] = new Room("Bright Hallway", "A stone hallway. You believe there must be an exit nearby due to a cold draught.");
        rooms[1]->addItem(new Item(rooms[0]->itemsInRoom.at(rooms[0]->isItemInRoom("shovel"))));
        //^ invokes deep copy constructor
    rooms[2] = new Room("Dingy Hallway", "Dark, gloomy and dirty. 2/10, terrible hallway.");
    rooms[3] = new Room("Dirty Hallway", "Perhaps the worst corridor you have had the honour of walking in. So dark a gru may be lurking nearby.");
    rooms[4] = new Room("Empty Room", "Hardly even cobwebs remain. Whatever was once in this room is now long gone.");
    rooms[5] = new Room("Filthy Hallway", "\"The light fixtures are functioning\" would be the kindest thing to say about this corridor.");
    rooms[6] = new Room("The Way Out", "An exit! Light leaks in from under a study, locked wooden door. Too heavy to break. "
                                       "But there seems to be a nearby puzzle mechanism that would unlock this door if you could solve it.\n"
                                       "[HINT: use the \"guess\" command with a five letter word to beat the Wordle-esque puzzle!]");
        rooms[6]->addPuzzle(new Zorkle());
    rooms[7] = new Room("Cells", "More cells, all empty. How did you end up in this place?");
    rooms[8] = new Room("Warden's Room", "This room's only feature of note is a strange journal which chronicles an attempt to create a peculiar "
                                         "form of lock for the exit door. Unfortunately, the location of this door is left unspecified."
                                         "[HINT: use the \"guess\" command to beat the puzzle!]");
        rooms[8]->addPuzzle(new Zorkle());
    rooms[9] = rooms[1];
    //^shallow copy constructor invoked

    //                  (N, E, S, W)
    rooms[0]->setExits(rooms[5], rooms[1], rooms[3], rooms[2]);
    rooms[1]->setExits(rooms[6], NULL, rooms[9], rooms[0]);
    rooms[2]->setExits(NULL, rooms[0], NULL, NULL);
    rooms[3]->setExits(rooms[0], rooms[4], NULL, rooms[8]);
    rooms[4]->setExits(NULL, NULL, NULL, rooms[3]);
    rooms[5]->setExits(NULL, rooms[6], rooms[0], rooms[7]);
    rooms[6]->setExits(NULL, NULL, NULL, rooms[5]);
    rooms[7]->setExits(NULL, rooms[5], NULL, NULL);
    rooms[8]->setExits(NULL, rooms[3], NULL, NULL);
    rooms[9]->setExits(rooms[1], NULL, NULL, NULL);

    currentRoom = rooms[0];
    printWelcome();
}

void ZorkUL::printWelcome() {
    w << QString("Welcome to Zorkle, the leading text based Wordle-like adventure game.");
    w << QString("Please use the GUI or enter commands in the dialogue box below to proceed.");
    w << QString("Press the question mark or enter \"info\" below for information on other commands.");
    w << QString("Enjoy the game!");
    w << QString::fromStdString(currentRoom->longDescription());
}

/** COMMANDS **/
void ZorkUL::printHelp() {
    w << QString("Valid commands are: ");
    w << QString::fromStdString(parser.showCommands());
}

void ZorkUL::goRoom(Command command) {
    if (!command.hasSecondWord()) {
        w << QString("Incomplete input.");
        return;
    }

    string direction = command.getSecondWord();

    // Try to leave current room.
    Room* nextRoom = currentRoom->nextRoom(direction);

    if (nextRoom == NULL)
        w << QString("Underdefined input.");
    else {
        currentRoom = nextRoom;
        w << QString::fromStdString(currentRoom->longDescription());
    }
}

string ZorkUL::go(string direction) {
    //Make the direction lowercase
    //transform(direction.begin(), direction.end(), direction.begin(),:: tolower);
    //Move to the next room
    Room* nextRoom = currentRoom->nextRoom(direction);
    if (nextRoom == NULL)
        return("Direction is null.");
    else
    {
        currentRoom = nextRoom;
        return currentRoom->longDescription();
    }
}


/**
 * Given a command, process (that is: execute) the command.
 * If this command ends the ZorkUL game, true is returned, otherwise false is
 * returned.
 */
bool ZorkUL::processCommand(Command command) {
    if (command.isUnknown()) {
        w << QString("Invalid input.");
        return false;
    }

    string commandWord = command.getCommandWord();
    if (commandWord.compare("info") == 0) printHelp();

    else if (commandWord.compare("inventory") == 0) {
        if (player->inventory.size() == 0) {
            w << QString("Your inventory is empty.");
        }
        else {
            w << QString("Your inventory contains: ");
            for (unsigned int i = 0; i < player->inventory.size(); i++) {
                w << QString::fromStdString(player->inventory.at(i)->getShortDescription());
            }
        }
    }

    else if (commandWord.compare("guess") == 0) {
        if (currentRoom->hasPuzzle()) {
            if (command.hasSecondWord()) {
                if (!currentRoom->puzzle->isCorrect() && currentRoom->puzzle->remainingGuesses > 0) {
                    currentRoom->puzzle->tryInput(command.getSecondWord());
                    vector<string> lines = currentRoom->puzzle->outputState();
                    unsigned int letterCount = 1;
                    for (unsigned int i = 0; i < lines.size(); i+=2) {
                        w->outputAppend(QString::fromStdString(lines[i] + " "), QString::fromStdString(lines[i+1]));
                        if (letterCount == 5) {
                            letterCount = 0;
                            w << QString(""); //outputs "\n"
                        }
                        letterCount++;
                    }
                    w << QString(""); //outputs "\n"
                    if (currentRoom->puzzle->isCorrect()) w << QString("Puzzle complete!");
                }
                else if (currentRoom->puzzle->isCorrect()) w << QString("Puzzle complete!");
                else {
                    w << QString("Unable to guess further. Puzzle has been reset.");
                    currentRoom->puzzle->Reset();
                }
            }
            else {
                w << QString("Underdefined input.");
            }
        }
        else {
            w << QString("This room does not have a puzzle.");
        }
    }

    else if (commandWord.compare("map") == 0) {
        //\u200B is a 0 length space character, used for formatting purposes here; leading whitespace is trimmed otherwise.
        w << QString("          [Cells] --- [Filthy Hallway] --- [The Way Out]\n"
              "\u200B                                       |                               |\n"
              "\u200B                                       |                               |\n"
                     "[Dingy Hallway] --- [Dungeon Cell] --- [Bright Hallway]\n"
              "\u200B                                       |         \n"
              "\u200B                                       |         \n"
                     "[Warden's Room] --- [Dirty Hallway] --- [Empty Room]");
    }

    else if (commandWord.compare("look") == 0) {
        w << QString::fromStdString(currentRoom->longDescription());
    }

    else if (commandWord.compare("go") == 0) {
        goRoom(command);
    }

    else if (commandWord.compare("take") == 0) {
        if (!command.hasSecondWord()) {
            w << QString("Incomplete input.");
        }
        else {
            w << QString::fromStdString("You're trying to take " + command.getSecondWord() + ".");
            int location = currentRoom->isItemInRoom(command.getSecondWord());
            if (location < 0 )
                w << QString("Item is not in the room.");
            else {
                w << QString::fromStdString("You add " + command.getSecondWord() + " to your inventory.");
                currentRoom->removeItemFromRoom(location);
                w << QString::fromStdString(currentRoom->longDescription());
                Item* i = &(currentRoom->itemsInRoom.at(location));
                player->addItem(i);
            }
        }
    }
    //deallocation of destroyed items for memory management purposes
    else if (commandWord.compare("destroy") == 0) {
        if (!command.hasSecondWord()) {
            w << QString("Incomplete input.");
        }
        else {
            w << QString::fromStdString("You're trying to destroy " + command.getSecondWord() + ".");
            int location = currentRoom->isItemInRoom(command.getSecondWord());
            if (location < 0 )
                w << QString("Item is not in the room.");
            else {
                w << QString::fromStdString("You destroyed the " + command.getSecondWord() + ".");
                currentRoom->removeItemFromRoom(location);
                w << QString::fromStdString(currentRoom->longDescription());
                currentRoom->itemsInRoom.erase(currentRoom->itemsInRoom.begin() + location);
            }
        }
    }


    /*
    else if (commandWord.compare("put") == 0) {

    }*/
    /*
    {
    if (!command.hasSecondWord()) {
        w << QString("incomplete input\n");
        }
        else
            if (command.hasSecondWord()) {
            w << string("you're adding " + command.getSecondWord() + "\n");
            itemsInRoom.push_Back;
        }
    }
*/
    //use of user-defined exception InvalidInputException
    else if (commandWord.compare("teleport") == 0) {
        try {
            if (!command.hasSecondWord()) {
                InvalidInputException e;
                throw e;
            }
            srand (time(NULL));
            int randomRoomIndex = rand() % ROOMCOUNT;
            currentRoom = rooms[randomRoomIndex];
            w << QString("Your vision goes white, and you find yourself displaced in space.");
            w << QString::fromStdString(currentRoom->longDescription());
        }  catch (InvalidInputException& i) {
            i.what();
        }
    }
    else if (commandWord.compare("quit") == 0) {
        w << QString("Quitting.");
        return true; //signal to quit
    }
    return false;
}
