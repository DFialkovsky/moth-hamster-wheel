#include "rawEntry.hpp"


rawEntry::rawEntry()
{
	number_ = 0;
	milliseconds_ = 0;
}

rawEntry::rawEntry(int number, unsigned int milliseconds)
{
	number_ = number;
	milliseconds_ = milliseconds;
}

 unsigned int rawEntry::getMilliseconds()
{
	return milliseconds_;
}

int rawEntry::getNumber()
{
	return number_;
}

void rawEntry::setMilliseconds(unsigned int milliseconds)
{
	milliseconds_ = milliseconds;
}


void rawEntry::setNumber(int number)
{
	number_ = number;
}


rawEntry::~rawEntry()
{

}
