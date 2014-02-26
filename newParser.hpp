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
#include <boost/date_time/posix_time/posix_time.hpp> // for getting timestamps
#include <boost/filesystem.hpp> // for creation of directories
#include <fstream> // ifstream
#include <iostream> // cout, cerr
#include <algorithm>
#include <limits> // max int
#include <utility> // std::pair
#include <cctype> // isdigit

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
    // Reads the file, parses it into individual parts, all of which start from zero
    // Prints them in a dump folder named "raw_dumps"
    // Input string filename, output will vary based on error found.
    // Output will be a std::pair(ERRMSG,LINENUMBER) 
    // if there is no error message output will be (0,0)
    std::pair<int,unsigned int> processFile(std::string fileName);
    void DumpEverything();
    
    
    // emits a xls file with the parsed data into in dumps/xls/
    // dumps/xls/ is created if it doesn't exist.
    void DumpXlsBook();
    // emits x csv files with the parsed data into in dumps/csv/
    // where x is the amount of unique sources in the given file
    // dumps/xls/ is created if it doesn't exist.
    void DumpCSVs();
    // dumps only the raw generated data used to create the spreadsheets
    void DumpRaws();
    
    // TODO: see if implementationis even necessary
    void DumpSpecificCylinder(int cylindernum)
    {
	// FIXME: I need an implementation
	return;
    }

private:
    // If the file is not in the structure, return val>0
    // return 0 if successful
    int copyFileToMem(std::string fileName, unsigned int & badLineNumber);
    void parseIntoSections();
    void removeReturnCarraiges(std::string & subjectedStr);
    
    //returns the first instance of object inside comparison. If noting is found, max int is returned
    unsigned int find_first_of(unsigned int object, std::vector< int > comparison);
    
    // checks if there are any non-letter characters in the string.
    // NOTE: '-','+' are also discriminated against!
    bool isNumber(std::string str);
private:
    std::string 		curFileName_;
    
    std::vector<timeEntry> 	timeEntries_;
    std::vector<int> 		cylinderList_;
    std::vector<dataEntry>	dataEntries_;
    
public:
    enum 
    {
	FILE_OK = 0,
	ERR_3PLUS_ENTRES_PER_LINE,
	ERR_1ENTRY_PER_LINE,
	ERR_TOXIC_CHARACTER,
	FILE_EMPTY,
	ERR_MILLIS_SHRUNK,
	ERR_MILLIS_CONSTANT	
    }; // end enum
};

#endif