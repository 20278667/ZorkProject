#include "item.h"

Item::Item (string inDescription, int inWeightGrams, float inValue, bool hidden) {
	description = inDescription;
	setWeight(inWeightGrams);
	value = inValue;
    this->hidden = hidden;
}

Item::Item(string inDescription) {
	description = inDescription;
}

//deep copy constructor
Item::Item(const Item* old) {
    description = string(old->description);
    setWeight(old->weightGrams);
    setValue(old->value);
    setHidden(old->hidden);
}

void Item::setWeight(int inWeightGrams)
{
    if (inWeightGrams > 9999 || inWeightGrams < 0)
       cout << "weight invalid, must be 0<weight<9999" ;
    else
	   weightGrams = inWeightGrams;
}

void Item::setValue(float inValue)
{
    if (inValue > 9999 || inValue < 0)
       cout << "value invalid, must be 0<value<9999" ;
    else
	   value = inValue;
}

bool Item::getHidden() {
    return hidden;
}

void Item::setHidden(bool hidden)
{
    this->hidden = hidden;
}

string Item::getShortDescription()
{
	return description;
}

string Item::getLongDescription()
{
	return " item(s), " + description + ".\n";
}

