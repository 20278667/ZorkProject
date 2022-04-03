#include "ZorkUL.h"
#include "mainwindow.h"

//data
MainWindow *w;
int roomCount = 9;
Room* rooms[9];

//flags
bool input = false;

//overloaded operator << for ease of sending data to MainWindow w for display.
inline MainWindow* operator<< (MainWindow *m, QString s) {
    m->output(s);
    return m;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new MainWindow();
    w->show();
    ZorkUL temp;
    thread t1(&ZorkUL::play, temp);
    int i = a.exec();
    t1.join();
    return i;
}


//constructor
ZorkUL::ZorkUL() {
    createRooms();
}

//main game loop
void ZorkUL::play() {
    w << QString::fromStdString(to_string(w->finished)) + QString("\n");
    w << QString::fromStdString(to_string(w->canPlay)) + QString("\n");
    w << QString::fromStdString(to_string(w->input)) + QString("\n");
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
                    // Free the memory allocated by "parser.getCommand()"
                    //   with ("return new Command(...)")
                    delete command;
                }
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    w << QString("\n end \n");
    w << QString("\n");
}


void ZorkUL::createRooms()  {

    rooms[0] = new Room("a");
        rooms[0]->addItem(new Item("x", 1, 11));
        rooms[0]->addItem(new Item("y", 2, 22));
    rooms[1] = new Room("b");
        rooms[1]->addItem(new Item("xx", 3, 33));
        rooms[1]->addItem(new Item("yy", 4, 44));
    rooms[2] = new Room("c");
    rooms[3] = new Room("d");
    rooms[4] = new Room("e");
    rooms[5] = new Room("f");
    rooms[6] = new Room("g");
    rooms[7] = new Room("h");
    rooms[8] = new Room("i");

    //                  (N, E, S, W)
    rooms[0]->setExits(rooms[5], rooms[1], rooms[3], rooms[2]);
    rooms[1]->setExits(NULL, NULL, NULL, rooms[0]);
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
    w << QString("start\n");
    w << QString("info for help\n");
    w << QString::fromStdString(currentRoom->longDescription()) + "\n";
}

/** COMMANDS **/
void ZorkUL::printHelp() {
    w << QString("valid inputs are;\n");
    parser.showCommands();

}

void ZorkUL::goRoom(Command command) {
    if (!command.hasSecondWord()) {
        w << QString("incomplete input\n");
        return;
    }

    string direction = command.getSecondWord();

    // Try to leave current room.
    Room* nextRoom = currentRoom->nextRoom(direction);

    if (nextRoom == NULL)
        w << QString("underdefined input");
    else {
        currentRoom = nextRoom;
        w << QString::fromStdString(currentRoom->longDescription() + string("\n"));
    }
}

string ZorkUL::go(string direction) {
    //Make the direction lowercase
    //transform(direction.begin(), direction.end(), direction.begin(),:: tolower);
    //Move to the next room
    Room* nextRoom = currentRoom->nextRoom(direction);
    if (nextRoom == NULL)
        return("direction null");
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
        w << QString("invalid input\n");
        return false;
    }

    string commandWord = command.getCommandWord();
    if (commandWord.compare("info") == 0)
        printHelp();

    else if (commandWord.compare("map") == 0)
        {
            w << QString("[h] --- [f] --- [g]\n");
            w << QString("         |         \n");
            w << QString("         |         \n");
            w << QString("[c] --- [a] --- [b]\n");
            w << QString("         |         \n");
            w << QString("         |         \n");
            w << QString("[i] --- [d] --- [e]\n");
        }

    else if (commandWord.compare("go") == 0)
        goRoom(command);

    else if (commandWord.compare("take") == 0)
    {
        if (!command.hasSecondWord()) {
        w << QString("incomplete input");
        }
        else if (command.hasSecondWord()) {
        w << QString::fromStdString("you're trying to take " + command.getSecondWord() + "\n");
        int location = currentRoom->isItemInRoom(command.getSecondWord());
        if (location  < 0 )
            w << QString("item is not in room\n");
        else {
            w << QString("item is in room\n");
            w << QString::fromStdString("index number " + std::to_string(location) + "\n\n");
            w << QString::fromStdString(currentRoom->longDescription() + "\n");
        }
        }
    }

    else if (commandWord.compare("put") == 0)
    {

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
            w << QString::fromStdString(currentRoom->longDescription() + "\n");
        }
        else {
            w << QString("overdefined input\n");
        }
    }
    else if (commandWord.compare("quit") == 0) {
        if (command.hasSecondWord())
            w << QString("overdefined input\n");
        else
            return true; /**signal to quit*/
    }
    return false;
}
