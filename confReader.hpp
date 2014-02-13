#ifndef _confReader_HPP_
#define _confReader_HPP_

#include <vector>
#include <fstream>
#include <string>
#include <cctype>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

static const double DEFAULT_CYLINDER_VAL = 15.0;

class confReader{
public:
    confReader() {};
    ~confReader() {};
    
private:
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    std::vector <double> cylinderData; 
    
public:
    // by default the confReader will look for a number like "cylinder5" and see the corresponding number. 
    //The number may be in decimals.
    static double getCylinderSize(int num, std::string propertiesPath = "properties.conf") {
	std::ifstream 	stream; // input stream
	std::string 	buffer;
	std::string 	temp;
	std::string 	number;
	int 		cylinderNum;
	double 		cylinderSize;
	
	stream.open(propertiesPath.c_str()); // open the file
	size_t location(0);
	while (std::getline(stream, buffer)) // while there is still stuff in the file, get a line from it and put it in buffer
	{
	    removeCarrageReturn(buffer); // find and kill any '\r'
	    boost::char_separator<char> seperators(",\t()| &^%!@$="); // all of these chars will be ignored and used a tokenizers
	    tokenizer tokens(buffer, seperators); // tokenize
	    
	    tokenizer::iterator tok = tokens.begin(); // there is a list of tokens made, it is accessible via iterator
	    if (tok == tokens.end()) // if there is nothing in the line
		continue; // skip the line
		
    	    temp = *tok; // string temp = dereference to tok (tok was pointing to a string)

	    if (temp[0] == '#') // this is a comment mark, if found, then ignore the line,
		continue; // skip the line
// 	    ++tok; // go to next token

	    temp = *tok; // string temp = dereference to token
	    
	    (toLower(temp)); // turn to all lower case
	    
	    if ((location = (temp).find("cylinder"))== std::string::npos){ // if that doesnt exist
		return DEFAULT_CYLINDER_VAL;
	    }
	    number = (temp).substr(location+8); // everything after cylinder
	    if (number.find_first_not_of("1234567890") != std::string::npos) // if anything that isnt those exists
	    {
		return DEFAULT_CYLINDER_VAL;
	    }
	    cylinderNum = boost::lexical_cast<int>(number);
	    if (num != cylinderNum)
		continue;
	    ++tok;
	    
	    number = *tok;
	    if (number.find_first_not_of("1234567890.") != std::string::npos) // if anything that isnt those exists
	    {
		return DEFAULT_CYLINDER_VAL;
	    }
	    cylinderSize = boost::lexical_cast<double>(number); // convert string to double
	    return cylinderSize;
	    
	    }
	    // if all else fails, return the default val
	    return 15.0;
    };
    
public:
    // scans a string and removes every instance of '\r' within
    static void removeCarrageReturn(std::string& str) 
    {
	size_t rKiller;
	while ((rKiller = str.find_first_of("\r")) != std::string::npos) // check for \r since we dont need them outside windows
	{
	    str.erase(rKiller, 1); // kill it where it stands, also kill any other ones if they exist
	}
    };
    // takes a whole string an makes sure all of its letters are lower case ascii
    static void toLower(std::string &input) {
	for (size_t i = 0; i < input.length(); ++i)
	{
	    if (input[i] >= 65 && input[i] <= 90)
		input[i] += 32;
	}
    }
    // takes a cstring and makes sure all of its letters are lower case ascii
    static void toLower(char * input) {
	for (int i = 0; i != '\0'; i++){
	    if (input[i] >= 65 && input[i] <= 90)
		input[i] += 32;
	}
    }
    
};

#endif // ifndef