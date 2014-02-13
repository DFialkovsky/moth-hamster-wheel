#ifndef _rawEntry_hpp_
#define _rawEntry_hpp_

class rawEntry {
public:
	rawEntry(): number_(0), milliseconds_(0) {}
	rawEntry(int number, unsigned int milliseconds): number_(number), milliseconds_(milliseconds) {}
public:
	const int getNumber()
	{
	    return number_;
	}
	const unsigned int getMilliseconds()
	{
	    return milliseconds_;
	}
	
	void setNumber(int number)
	{
	    number_ = number;
	}
	void setMilliseconds(unsigned int milliseconds)
	{
	    milliseconds_ = milliseconds;
	}
	
private:
	int 		number_;
	unsigned int 	milliseconds_;
};


#endif
