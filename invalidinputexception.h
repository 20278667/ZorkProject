#ifndef INVALIDINPUTEXCEPTION_H
#define INVALIDINPUTEXCEPTION_H

#include <exception>

using namespace std;

class InvalidInputException : public exception{
public:
    const char * what() const throw()
    {
        return "Command not understood.\n";
    }
};

#endif // INVALIDINPUTEXCEPTION_H
