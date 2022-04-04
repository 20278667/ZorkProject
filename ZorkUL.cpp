#include "ZorkUL.h"
#include "mainwindow.h"
#include "zorkle.h"

//data
MainWindow *w;
QApplication *a;
const int roomCount = 9;
Room* rooms[roomCount];

//flags
bool input = false;

//overloaded friend inline operator << for ease of sending data to MainWindow w for display.
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
                    if (!(command->getCommandWord().empty())) {
                        // Pass dereferenced command and check for end of game.
                        w->finished = processCommand(*command);
                    }
                    // Free the memory allocated by "parser.getCommand()" with ("return new Command(...)")
                    delete command;
                }
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    w << QString("End.");
    a->quit();
}


void ZorkUL::createRooms()  {

    rooms[0] = new Room("Dungeon Cell", "A dank and dusty dungeon. Every surface is covered with a thick layer dirt and grime.");
        //rooms[0]->addItem(new Item("x", 1, 11));
    rooms[1] = new Room("Bright Hallway", "A stone hallway. You believe there must be an exit nearby due to a cold draught.");
    rooms[2] = new Room("Dingy Hallway", "Dark, gloomy and dirty. 2/10, terrible hallway.");
    rooms[3] = new Room("Dirty Hallway", "Perhaps the worst corridor you have had the honour of walking in. So dark a gru may be lurking nearby.");
    rooms[4] = new Room("Empty Room", "Hardly even cobwebs remain. Whatever was once in this room is now long gone.");
    rooms[5] = new Room("Filthy Hallway", "\"The light fixtures are functioning\" would be the kindest thing to say about this corridor.");
    rooms[6] = new Room("The Way Out", "An exit! Light leaks in from under a study, locked wooden door. Too heavy to break."
                                       "But there seems to be a puzzle mechanism that would unlock this door.");
        rooms[6]->addPuzzle(new Zorkle());
    rooms[7] = new Room("Cells", "More cells, all empty. How did you end up in this place?");
    rooms[8] = new Room("Warden's Room", "This room's only feature of note is a strange journal which chronicles an attempt to create a peculiar "
                                         "form of lock for the exit door. Unfortunately, the location of this door is left unspecified.");
        rooms[8]->addPuzzle(new Zorkle());

    //                  (N, E, S, W)
    rooms[0]->setExits(rooms[5], rooms[1], rooms[3], rooms[2]);
    rooms[1]->setExits(rooms[6], NULL, NULL, rooms[0]);
    rooms[2]->setExits(NULL, rooms[0], NULL, NULL);
    rooms[3]->setExits(rooms[0], rooms[4], NULL, rooms[8]);
    rooms[4]->setExits(NULL, NULL, NULL, rooms[3]);
    rooms[5]->setExits(NULL, rooms[6], rooms[0], rooms[7]);
    rooms[6]->setExits(NULL, NULL, NULL, rooms[5]);
    rooms[7]->setExits(NULL, rooms[5], NULL, NULL);
    rooms[8]->setExits(NULL, rooms[3], NULL, NULL);

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
    if (commandWord.compare("info") == 0)
        printHelp();

    else if (commandWord.compare("map") == 0) {
        //\u200B is a 0 length space character, used for formatting purposes here; leading whitespace is trimmed otherwise.
        w << QString("      [h] --- [f] --- [g]");
        w << QString("\u200B         |         ");
        w << QString("\u200B         |         ");
        w << QString("      [c] --- [a] --- [b]");
        w << QString("\u200B         |         ");
        w << QString("\u200B         |         ");
        w << QString("      [i] --- [d] --- [e]");
    }

    else if (commandWord.compare("look") == 0) {

    }

    else if (commandWord.compare("go") == 0) {
        goRoom(command);
    }

    else if (commandWord.compare("take") == 0) {
        if (!command.hasSecondWord()) {
            w << QString("Incomplete input.");
        }
        else if (command.hasSecondWord()) {
            w << QString::fromStdString("You're trying to take " + command.getSecondWord());
            int location = currentRoom->isItemInRoom(command.getSecondWord());
            if (location < 0 )
                w << QString("Item is not in the room.");
            else {
                w << QString("Item is in the room.");
                w << QString::fromStdString("Index number: " + to_string(location));
                w << QString::fromStdString(currentRoom->longDescription());
            }
        }
    }

    else if (commandWord.compare("put") == 0) {

    }
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
    else if (commandWord.compare("teleport") == 0) {
        if (!command.hasSecondWord()) {
            srand (time(NULL));
            int randomRoomIndex = rand() % roomCount;
            currentRoom = rooms[randomRoomIndex];
            w << QString("Your vision goes white, and you find yourself displaced in space.");
            w << QString::fromStdString(currentRoom->longDescription());
        }
        else {
            w << QString("Overdefined input.");
        }
    }
    else if (commandWord.compare("quit") == 0) {
        w << QString("Quitting.");
        return true; //signal to quit
    }
    return false;
}
