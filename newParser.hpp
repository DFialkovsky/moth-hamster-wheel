/*
 *Daniel Fialkovsky
 *
 *BSD Lisence
 *
 *Just a quick note for now
 *
 *my class delclarations are as follows:
 *class
 *   public: constructors
 v   public: methods
 *   private: methods
 *   private members (end with _)
 *  in that order
 * 
 *
 */

#ifndef _newParser_
#define _newParser_

#include <string>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <limits>

#include "rawEntry.hpp"
#include "dataEntry.hpp"

#include "confReader.hpp"

#include "filePrinter.hpp" // for printing csv files
#include "xlslib/xlslib.h" // for printing xls spreadsheet files  

static const unsigned int MAX_UINT32 = std::numeric_limits<unsigned int>::max();

typedef rawEntry timeEntry;
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

class newParser
{
public:
    newParser();
    ~newParser();
public:
    // reads the file, parses it into individual parts, all of which start from zero
    // prints them in a dump folder named "raw_dumps"
    bool processFile(std::string fileName);
    void DumpEverything();
    
    void DumpXlsBook();
    
    // dumps only the raw generated data used to create the spreadsheets
    void DumpRaws();
    
    // TODO: see if implementationis even necessary
    void DumpSpecificCylinder(int cylindernum)
    {
	// FIXME: I need an implementation
	return;
    }

private:
    // If the file is not in the structure, 
    void copyFileToMem(std::string fileName, bool fileOk, unsigned int badLineNumber);
    void parseIntoSections();
    void removeReturnCarraiges(std::string & subjectedStr);
    
    //returns the first instance of object inside comparison. If noting is found, max int is returned
    unsigned int find_first_of(unsigned int object, std::vector< int > comparison);

private:
    std::string 		curFileName_;
    
    std::vector<timeEntry> 	timeEntries_;
    std::vector<int> 		cylinderList_;
    std::vector<dataEntry>	dataEntries_;
    

};

#endif