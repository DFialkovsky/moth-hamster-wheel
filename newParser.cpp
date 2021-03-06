#include "newParser.hpp"


newParser::newParser(): curFileName_()
{

}

std::pair<int,unsigned int> newParser::processFile(std::string fileName)
{
    unsigned int errLineNum(0);
    int outputStatus = copyFileToMem(fileName,errLineNum);
    if (outputStatus) // if !=0
    {
	// TODO: display error message
	return std::pair<int, unsigned int> (outputStatus, errLineNum);
    }
    parseIntoSections();
    DumpXlsBook();
    DumpCSVs();
    return std::pair<int, unsigned int> (FILE_OK,0);
}


// bad Line number double as a line counter which will be important to the user if fileOk turns false
int newParser::copyFileToMem(std::string fileName, unsigned int & badLineNumber)
{
  std::ifstream fileStream;
  std::string buffer;
  std::string time, cylinder;
  timeEntry tempEntry;
  int cylinderInt(0);
  unsigned int timeInt(0);
  
  int prevCylinderInt(0);
  int prevtimeInt(0);
  bool pastFirstIteration(false);
  // these are the tokens that will seperte each entry
  
  boost::char_separator<char> seperators(" ,\t");
  if (!fileStream.is_open()) {
    fileStream.open(fileName.c_str());
  

	while (std::getline(fileStream,buffer)) { // while there is content in the file, get line
	    badLineNumber++;
	    // if a windows return carraige character exists,  remove it to make the output portable to 
	    // to UNIX-like operating systems

	    #ifndef _WIN32 // if windows not defined
	    removeReturnCarraiges(buffer);
	    #endif
	    
	    // NOTE: expected format of the file is time,cylinder#.

	    tokenizer tokens(buffer, seperators);
	    tokenizer::iterator tok = tokens.begin();

	    if (tok == tokens.end()) // if empty string, ignore
		continue;
	    time =  *(tok);
	    ++tok;
	    
	    if (tok == tokens.end()) // check if line only has 1 entry instead of 2.
	    {
		return ERR_1ENTRY_PER_LINE;
	    }
	    cylinder = *(tok);
	    
	    if (++tok != tokens.end()) // there are too many contents in a single line.
	    {
		return ERR_3PLUS_ENTRES_PER_LINE;
	    }
	    
	    if (!(isNumber(cylinder) || isNumber(time))) // if either section contains non letters
	    {
		return ERR_TOXIC_CHARACTER;
	    }
	    cylinderInt = boost::lexical_cast<int>(cylinder);
	    timeInt = boost::lexical_cast<unsigned int>(time);

	    // Look at the current cylinder number, if a new one is found, add it to the CylinderList
	    // otherwise ignore it. 
	    if (find_first_of(cylinderInt, cylinderList_) == MAX_UINT32) {
		cylinderList_.push_back(cylinderInt);
	    }
	    if (pastFirstIteration)
	    {
		if (prevtimeInt > timeInt ) // if the millisecond clock is broken, then the data is probably bad.
		    return ERR_MILLIS_SHRUNK;
		
		if (prevtimeInt == timeInt && prevCylinderInt == cylinderInt) // if there is an exact repeat, the entry.
		    continue;
	    }
	    prevCylinderInt = cylinderInt;
	    prevtimeInt = timeInt;
	    
	    
	    tempEntry.setNumber(cylinderInt);
	    tempEntry.setMilliseconds(timeInt);
	    timeEntries_.push_back(tempEntry);
	    
	    pastFirstIteration = true;
	    
	} // end while
  
    } // end if the fileStream isnt open
    fileStream.close();
    fileStream.clear();
    
    std::sort(cylinderList_.begin(), cylinderList_.end());
    return FILE_OK;
}

void newParser::DumpRaws()
{
    for (int i = 0; i < cylinderList_.size(); i++)
    {
	dataEntries_[i].printRawData();
    }
}


void newParser::parseIntoSections()
{
  unsigned int entriesCount = cylinderList_.size();
  dataEntries_.resize(entriesCount);
  double radius;
  
  std::string outputBuffer;
  
  for (int i = 0; i < entriesCount; i++) {
	dataEntries_[i].setCylinderNum(cylinderList_[i]);
	std::cerr << i << "\n";

	radius = confReader::getCylinderSize(cylinderList_[i]);
	dataEntries_[i].setRadius(radius);
	
	dataEntries_[i].getFullDataSheet(timeEntries_);
	dataEntries_[i].computeBouts();
  }
}


unsigned int newParser::find_first_of(unsigned int  object, std::vector< int > comparison)
{

    for (unsigned int i = 0; i < comparison.size(); i++) {
      if (object == comparison[i]) {
	return i;
	
      } // end if
    } // end for
    
  return MAX_UINT32;
}

void newParser::removeReturnCarraiges(std::string& subjectedStr)
{
    std::size_t slashRHunter(0);
    while ((slashRHunter = subjectedStr.find('\r')) != std::string::npos) 
    {
	subjectedStr.erase(slashRHunter,  1); // erase the \r only
    }
		
}


bool newParser::isNumber(std::string str)
{
    for (std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
	if (!std::isdigit(*it))
	{
	    return false;
	}
    }
    return true;
}


void newParser::DumpCSVs()
{
    for (int i = 0; i < cylinderList_.size(); i++)
    {
	dataEntries_[i].printProcessedDataCSV();
    }
}


void newParser::DumpXlsBook()
{
//     std::vector<xlslib_core::worksheet*> worksheets;
    xlslib_core::workbook wb;
 
    int cylinderCount = cylinderList_.size();
    std::string cylinderNumber;
    xlslib_core::worksheet* worksheets[cylinderCount];
    
    // initilize each worksheet and connect them to wb
    for (int i = 0; i < cylinderCount; i++)
    {
	cylinderNumber = "Mill#";
	cylinderNumber += boost::lexical_cast<std::string>(cylinderList_.at(i));
	worksheets[i] = wb.sheet(cylinderNumber);
    }
    std::string varianceAvgSpeed, varianceDistanceCvrd, varianceTimeElpsd;
    
    boost::posix_time::ptime Sectime(boost::posix_time::second_clock::local_time());
    std::string currentTime = boost::posix_time::to_simple_string(Sectime);
    for (int i = 0; i < cylinderCount; i++)
    {

	std::vector<boutEntry> dataEntryDump = dataEntries_[i].getBoutEntriesList();
/*----------------------------set column width---------------------------------*/
	worksheets[i]->defaultColwidth(8);
	worksheets[i]->colwidth(2,4700); // i think 3000 is the default size.
	worksheets[i]->colwidth(3,4000); 
	worksheets[i]->colwidth(5,4000);
	worksheets[i]->colwidth(7,4000);
	
/*----------------------------set date time header in first line---------------*/

	worksheets[i]->label(0,0,"Date Created: ");
	worksheets[i]->merge(0,0, 0,1);
	worksheets[i]->merge(0,2, 0,3);
	worksheets[i]->merge(0,2, 0,4);
	worksheets[i]->merge(0,4, 0,5);
	worksheets[i]->label(0,2, currentTime);
/*----------------------------set set header for data--------------------------*/

	worksheets[i]->label(4,0,"Bout Range");
	worksheets[i]->label(4,1,"Laps");
	worksheets[i]->label(4,2,"Distance Covered(cm)");
	worksheets[i]->label(4,3,"Var.distCvrd");
	worksheets[i]->label(4,4,"Avg Speed");
	worksheets[i]->label(4,5,"Var.AvgSpd");
	worksheets[i]->label(4,6,"Time Elapsed");
	worksheets[i]->label(4,7,"Var.TimeElpsd");
	
	varianceDistanceCvrd = "=VAR(C6:C";
	varianceDistanceCvrd += boost::lexical_cast<std::string>(dataEntryDump.size()+5);
	varianceDistanceCvrd += ")";
  
	varianceAvgSpeed = "=VAR(E6:E";
	varianceAvgSpeed += boost::lexical_cast<std::string>(dataEntryDump.size()+5);
	varianceAvgSpeed += ")";

	varianceTimeElpsd = "=VAR(G6:G";
	varianceTimeElpsd += boost::lexical_cast<std::string>(dataEntryDump.size()+5);
	varianceTimeElpsd += ")";
	    
	worksheets[i]->label(5,3,varianceDistanceCvrd);
	worksheets[i]->label(5,5,varianceAvgSpeed);
	worksheets[i]->label(5,7,varianceTimeElpsd);
	
	
	unsigned int totalBouts = dataEntryDump.size();	
	std::string entryRange;

	for (unsigned int j = 0; j < totalBouts; j++)
	{
	    entryRange = boost::lexical_cast<std::string>(dataEntryDump[j].entryRange.first +2);
	    entryRange += ':';
	    entryRange += boost::lexical_cast<std::string>(dataEntryDump[j].entryRange.second +2);
	    
	    worksheets[i]->label(j+5, 0, entryRange);
	    
	    worksheets[i]->number(j+5, 1, dataEntryDump[j].laps);
	    
	    worksheets[i]->number(j+5, 2, dataEntryDump[j].distanceCovered);
	    
	    worksheets[i]->number(j+5, 4, dataEntryDump[j].avgSpeed);

	    worksheets[i]->number(j+5, 6, dataEntryDump[j].timeElapsed);
	}
	
	
    }

/** -----------------------figure out current system time ------------------------**/
    boost::posix_time::ptime time(boost::posix_time::microsec_clock::local_time());
    std::string timeStr = boost::posix_time::to_simple_string(time);
    std::size_t loc;
    while ((loc = timeStr.find(':')) != std::string::npos) 
    {
	    timeStr.erase(timeStr.begin()+loc);
    }
/** -----------------------folder creation and file writing-----------------------**/

    boost::filesystem::path currentPath("dumps");
    boost::filesystem::create_directory(currentPath);

    std::string fileName = currentPath.string();
    fileName.reserve(37); // the string size should be around this size
    fileName += "/";
    fileName += timeStr;
    fileName += ".xls";
    wb.Dump(fileName);
}



newParser::~newParser()
{

}
