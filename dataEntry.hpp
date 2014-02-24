/* 
 * Daniel Fialkovsky
 * Created on 5/30/13
 * 

*/

#ifndef _dataEntry_hpp_
#define _dataEntry_hpp_

#include <iostream> 
#include <utility> //std::pair
#include <vector>
#include "rawEntry.hpp"
#include "filePrinter.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp> // pi
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/filesystem.hpp>

#include "xlslib/xlslib.h"
typedef rawEntry 		timeEntry;


typedef std::pair <unsigned int, unsigned int> twoUInts;

static const double PI =  boost::math::constants::pi<double>();
static const unsigned int LENGTH_OF_BOUT = 60000; // in milliseconds 60k = 1 minute


struct boutEntry {
  double 		distanceCovered;
  unsigned int 		timeElapsed;
  unsigned int 		laps;
  double		avgSpeed;
  twoUInts		entryRange;
};

typedef std::vector < boutEntry> boutEntries;

struct boutVarienceEntry {
  double varianceDistanceCovered;
  double varianceTimeElapsed;
  double varianceLaps;
  double varianceAvgSpeed;
};
 
typedef std::vector <boutVarienceEntry> boutVarienceEntries;


class dataEntry {
  
public:
    dataEntry();
    dataEntry(int cylinder, double radius);
    ~dataEntry();
  
public:
    // Move data (ID, size\n) into the dataEntry object.
    // input non-empty, initialized vector of timeEntries
    void getFullDataSheet(std::vector<timeEntry> & dataSheet);
    
    // sets the ID of this object to an integer, cylinder. This will be used when printing data files
    void setCylinderNum(int cylinder);
    // sets the radius variable to the double, radius.
    // used for computing the outputs
    void setRadius(double radius);
    // clear and reset everything in this type.
    void reset();
    // prints the contents of the primary raw data through stdout
    void printBlob();
    // prints the contents of the primary raw data into a file
    // this file is located in the ./raw folder. It will be created if it doesn't exist.
    void printRawData();
    // print all of the data generated that lives in memory into a comma delinated csv file
    // this file will be located in dumps/csv/
    void printProcessedDataCSV();
    // print all of the data generated that lives in memory into an xls file using xlslib
    // this file will be located in dumps/xls/
    void printProcessedDataXLS();
    // this will find the first entry in the vector that contains both zero's 
    // in the timeEntry slot
    static unsigned int find_first_of_marked(unsigned int object, std::vector<timeEntry> & comparison);

  
public:
    unsigned int		getLaps();
    // takes the last Entry in the vector and divides it by size_(#entries)
    double			getComputedAverageLaps();
    double			getComputedVarianceOfLaps();
    double 			getComputedVarianceUInt(std::vector<unsigned int> & vec);
    double 			getComputedVarianceDouble(std::vector<double> & vec);
    double			getComputedTotalDistanceFlown();
    unsigned int		getduration();
    void 			computeBouts();
  
    std::vector < boutEntry>	getBoutEntriesList();
private:
    // size keeps track of the number of laps as well
    unsigned int 		sizeofDataSheet_;
    int 			cylinderNum_;
    double 			radius_;
    std::vector <timeEntry> 	localDataSheet_;
    boutEntries			bouts_;		
    boutVarienceEntries		varianceBouts_;
    std::vector <twoUInts>	boutTimeRanges_;
    std::string 		fileToWrite_;
private:
    void 			generateTimeForFileName();
    double 			getFlightSpeedOnRotation(double timeFirst, double timeLast);
    void 			fillBoutEntry(int begin, int end);
    double 			avgSpeedVaraince();
    // in linux adds a '/', in windows adds a '\\' to the string
    inline static void 		addFolderCharTo(std::string & subjectedStr);
};


#endif