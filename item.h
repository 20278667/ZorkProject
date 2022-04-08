#ifndef ITEM_H_
#define ITEM_H_

#include <map>
#include <string>
#include <iostream>
using namespace std;

class Item {
private:
	string description;
	string longDescription;
	int weightGrams;
	float value;
    bool hidden;

    //initialiser list constructor
public:
    Item (string desc, int inWeight, float inValue, bool isHidden = false):description(desc), weightGrams(inWeight), value(inValue), hidden(isHidden) { };
    Item (string description);
    Item (const Item*);
	string getShortDescription();
    string getLongDescription();
	int getWeight();
	void setWeight(int weightGrams);
	float getValue();
	void setValue(float value);
    bool getHidden();
    void setHidden(bool hidden);
};

#endif /*ITEM_H_*/
