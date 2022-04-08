#include "CommandWords.h"

vector<string> CommandWords::validCommands;

/**
 * Constructor - initialise the command words.
 */
CommandWords::CommandWords() {
    // Populate the vector if we haven't already.
    if (validCommands.empty()) {
        validCommands.push_back("go");
        validCommands.push_back("quit");
        validCommands.push_back("info");
        validCommands.push_back("map");
        validCommands.push_back("guess");
        validCommands.push_back("look");
        validCommands.push_back("teleport");
        validCommands.push_back("take");
        validCommands.push_back("destroy");
        validCommands.push_back("inventory");
    }
}

/**
 * Check whether a given String is a valid command word.
 * Return true if it is, false if it isn't.
 **/
bool CommandWords::isCommand(string aString) {
    for (unsigned int i = 0; i < validCommands.size(); i++)
    {
        if (validCommands[i].compare(aString) == 0)
            return true;
    }
    // if we get here, the string was not found in the commands
    return false;
}

string CommandWords::showAll() {
    //Loops through validCommands and adds each to the output string.
    string out;
    for (unsigned int i = 0; i < validCommands.size(); i++)
    {
        out += validCommands[i] + ", ";
    }
    return out;
}
