#include "dataEntry.hpp"


dataEntry::dataEntry()
{
  generateTimeForFileName();
  bouts_.clear();
}


dataEntry::dataEntry(int cylinder, double radius)
{
  cylinderNum_ = cylinder;
  radius_ = radius;
  bouts_.clear();
  generateTimeForFileName();
}


void dataEntry::addFolderCharTo(std::string& subjectedStr)
{
#ifdef __gnu_linux__
    subjectedStr += "/";
#elif _WIN32
    subjectedStr += "\\";
#endif
}



void dataEntry::getFullDataSheet(std::vector< timeEntry > & dataSheet)
{
//   std::vector <int> copyList; 
  // copy the pointer to the vector. Whatever is changed to memorySheet_ will change dataSheet
//   memorySheet_ = dataSheet;

  unsigned int entriesToKill(0);
  long unsigned int subtractor(0);

  for (std::vector <timeEntry>::iterator it = dataSheet.begin(); it != dataSheet.end(); ++it) {
    if (it->getNumber() == cylinderNum_) {
      subtractor = it->getMilliseconds();
      break;
    }
  }
  std::cerr << "substractor is: " << subtractor << "\n";
  long unsigned int temp;
  for (std::vector <timeEntry>::iterator it = dataSheet.begin(); it != dataSheet.end(); ++it) {
    
    
	if (it->getNumber() == cylinderNum_) {
		temp = it->getMilliseconds() - subtractor;
		it->setMilliseconds(temp);
		localDataSheet_.push_back(*it );
		it->setNumber(0);
		it->setMilliseconds(0);
		++entriesToKill;
	} // end if
	
  
  }
  sizeofDataSheet_ = entriesToKill;
  unsigned int toKill;
  for (unsigned int i = 0; i < entriesToKill; i++) {
	if ((toKill = find_first_of_marked(0,dataSheet)) +1 != 0) {
	   dataSheet.erase(dataSheet.begin()+toKill);
	}
  }
//   int counter = 0;
//   for (std::vector<timeEntry>::iterator it = localDataSheet_.begin(); it != localDataSheet_.end(); ++it) {
//     std::cerr << it->getNumber() << " " << it->getMilliseconds() << "\n";
//     counter++;
//     
//   }
//   
//   
//   std::cerr << localDataSheet_.size() << "\n";
//   localDataSheet_.resize(counter);
}


unsigned int dataEntry::find_first_of_marked(unsigned int object, std::vector< timeEntry > & comparison)
{
    for (unsigned int i = 0; i < comparison.size(); i++) {
      if (object == comparison[i].getNumber() && object == comparison[i].getMilliseconds()) {
	return i;
	
      }
    }
    std::cerr << "FATAL ERROR\n";
  return 0xffffffff;
}



void dataEntry::printBlob()
{
//   for (auto it = localDataSheet_.begin(); it != localDataSheet_.end(); ++it) {
// 	  std::cerr << it->getNumber() << " " << it->getMilliseconds() << "\n";
//     
//   }
//   localDataSheet_.resize(4);
  std::cerr << localDataSheet_.size() << "\n";
  
  for (int i = 0; i < localDataSheet_.size(); i++)
    std::cerr << localDataSheet_[i].getNumber() << " " << localDataSheet_[i].getMilliseconds() << "\n";
}



void dataEntry::setCylinderNum(int cylinder)
{
  cylinderNum_ =cylinder;
}

void dataEntry::reset()
{
  localDataSheet_.clear();
}


void dataEntry::setRadius(double radius)
{
  radius_ = radius;
}



unsigned int dataEntry::getLaps()
{
  return sizeofDataSheet_;
}

double dataEntry::getComputedAverageLaps()
{
  unsigned int lastTimeEntry =  (--localDataSheet_.end())->getMilliseconds();
  return double(lastTimeEntry) / double(sizeofDataSheet_);
}

double dataEntry::getComputedTotalDistanceFlown()
{
  return double(sizeofDataSheet_) * 2.0*PI*radius_;
}

double dataEntry::getComputedVarianceOfLaps()
{
  double mean = getComputedAverageLaps();
  double variance(0.0), temp(0.0);
  
  for (std::vector<timeEntry>::iterator it = localDataSheet_.begin(); it != localDataSheet_.end(); ++it) {
	temp = double(it->getMilliseconds());
	temp -= mean;
	temp = std::pow(temp, 2);
	temp /= double(sizeofDataSheet_-1);
	variance += temp;
  }
  return variance;
}

double dataEntry::getComputedVarianceUInt(std::vector< unsigned int >& vec)
{
  double mean(0.0), temp(0.0), variance(0.0);
  unsigned int countSize(0);
  for (std::vector<unsigned int>::iterator it = vec.begin(); it != vec.end(); ++it) {
	mean += double(*it); 
	++countSize; // we save some time by not calling vec.size()
  } // end for
  mean /= countSize;
  
  for (std::vector<unsigned int>::iterator it = vec.begin(); it != vec.end(); ++it) {
	temp = double(*it);
	temp -= mean;
	temp = std::pow(temp, 2);
	temp /= double(countSize);
	variance += temp;
  } // end for 
  
  return variance;
}


double dataEntry::getComputedVarianceDouble(std::vector< double >& vec)
{
//   double mean(0.0), temp(0.0), variance(0.0);
//   unsigned int countSize(0);
//   for (std::vector<double>::iterator it = vec.begin(); it != vec.end(); ++it) {
// 	mean += double(*it); 
// 	++countSize; // we save some time by not calling vec.size()
//   } // end for
//   mean /= countSize;
//   
//   for (std::vector<double>::iterator it = vec.begin(); it != vec.end(); ++it) {
// 	temp = *it;
// 	temp -= mean;
// 	temp = std::pow(temp, 2);
// 	temp /= double(countSize);
// 	variance += temp;
//   } // end for 
//   boost::accumulators::accumulator_set<double, boost::accumulators::stats<tag::variance> > acc;
//   std::for_each(vec.begin(), vec.end(), bind void)
//   
//   return variance;
    int size(0);
    double mean(0.0);
    for (std::vector<double>::iterator it = vec.begin(); it != vec.end(); ++it) {
	mean += double(*it); 
	size++; // we save some time by not calling vec.size()
    }
    double temp = 0;
    for (int i = 0; i < size; i++)
    {
	temp += (mean-vec.at(i)*(mean-vec.at(i)));
	
    }
    return temp/size;
           
}



unsigned int  dataEntry::getduration()
{
  if (localDataSheet_.empty())
    return 0;
  //else
    return (--localDataSheet_.end())->getMilliseconds();
}

double dataEntry::getFlightSpeedOnRotation(double timeFirst, double timeLast)
{
  return  2.0*PI*radius_ / ((timeLast - timeFirst)/1000);
  
}

void dataEntry::computeBouts()
{
  twoUInts temp;
  std::pair<int, int> vectorRange;
  unsigned int lineNum(0);
  bool isNewBout(true);
  while (lineNum < sizeofDataSheet_) 
  {
	if (isNewBout) 
	{
	    isNewBout = false;
	    temp.first = localDataSheet_[lineNum].getMilliseconds();
	    vectorRange.first = lineNum;
	}
	// if the next line exists, and the difference is over a minute
	if (lineNum +1 <= sizeofDataSheet_)
	{
	    if(localDataSheet_[lineNum+1].getMilliseconds() - localDataSheet_[lineNum].getMilliseconds() > LENGTH_OF_BOUT)
	    {
		temp.second = localDataSheet_[lineNum].getMilliseconds();
		vectorRange.second = lineNum;
		 
		if (vectorRange.second -  vectorRange.first > 0) {
		    fillBoutEntry(vectorRange.first, vectorRange.second);
	// // 		std::cerr << "start: "  << vectorRange.first << " end: " << vectorRange.second <<  "\n"
	// // 			  << lineNum << ") first: " << temp.first << " 2nd: " << temp.second << "\n"; 
		    boutTimeRanges_.push_back(vectorRange);
		    vectorRange.first = 0;
		    vectorRange.second = 0;
		} // end if 

		// if the above if statement failed, then the bout is empty, set a new bout
		isNewBout = true;
	    }  // end if 
	} // end if lineNum +1 <= sizeofDataSheet_
	++lineNum;

    } // end while
    
}


void dataEntry::generateTimeForFileName()
{
  boost::posix_time::ptime time(boost::posix_time::microsec_clock::local_time());
  currentTimeStr_ = boost::posix_time::to_simple_string(time);
  std::size_t loc;
  while ((loc = currentTimeStr_.find(':')) != std::string::npos) {
	currentTimeStr_.erase(currentTimeStr_.begin()+loc);
  }  
}


void dataEntry::fillBoutEntry(int begin, int end)
{
  // If the size is zero, dont bother doing the rest of the function
  std::cerr << "begin: " << begin << " end: " << end << "\n";
  if (end - begin == 0) {
    return;
  }
  
  std::vector <unsigned int> 	entry;
  std::vector <double>		speedPerEntry;

  // for the meantime the localDataSheet will be handled directly in case if the sheet will be eaten from in a future release
  for (unsigned int i = begin; i <= end; i++) {
    entry.push_back(localDataSheet_[i].getMilliseconds());
  }
  boutEntry bEntry;
  
  bEntry.laps = end-begin;
  bEntry.distanceCovered = bEntry.laps * 2.0*PI*radius_;
  double speedAvg(0.0);
  for (unsigned int i = 0; i < end-begin; i++) {
    speedPerEntry.push_back(getFlightSpeedOnRotation(entry[i], entry[i+1]));
    speedAvg += getFlightSpeedOnRotation(entry[i], entry[i+1]);
  }
  speedAvg /= double(end-begin+1);
  bEntry.avgSpeed = speedAvg;
  bEntry.timeElapsed = *(--entry.end()) - (*entry.begin());
  bEntry.entryRange.first = begin;
  bEntry.entryRange.second = end;
//   std::cerr << "laps: " << bEntry.laps << " distanceCovered: " << bEntry.distanceCovered << " avgSpeed: "
// 	    << bEntry.avgSpeed << " timeElapsed: " << bEntry.timeElapsed << " last entry: " <<entry.back() <<  "\n";
 
  bouts_.push_back(bEntry);
//   std::cerr << "bouts size: " << bouts_.size() << "\n";
}


void dataEntry::printRawData()
{
    /**-----------setup file to print------------**/
    filePrinter printer;
    boost::filesystem::path currentPath("raws");
    boost::filesystem::create_directory(currentPath);
    std::string fileName = currentPath.string();
    fileName.reserve(47); // expected size is 45-47
    addFolderCharTo(fileName);
    fileName += "Raw-Cylinder-";
    fileName += boost::lexical_cast<std::string>( cylinderNum_);
    fileName += "-";
    fileName += currentTimeStr_;
    printer.newFile(fileName);
    /**-----------file writing------------**/

    std::stringstream sstream;
    sstream << "Cylinder Milliseconds\n";
    // the writeStream must accept a converted stringstream, I have NO CLUE why it cant take a std::string from a Cstr
    std::string str = sstream.str();
    sstream.str(std::string());
    printer.writeLine(str);
    for (int i = 0; i < localDataSheet_.size(); i++) 
    {
	sstream << localDataSheet_[i].getNumber() << ",\t" << localDataSheet_[i].getMilliseconds() << "\n";
	str = sstream.str();
	sstream.str(std::string());
	printer.writeLine(str);
    }

}

void dataEntry::printProcessedDataCSV()
{
    computeBouts();
    
    boost::filesystem::path currentPath("dumps");
    boost::filesystem::create_directory(currentPath);

    std::string fileName = currentPath.string();
    addFolderCharTo(fileName); // add '/' or '\\' depending on platform
    fileName.reserve(48); // expected size is 46-48
    fileName+= "Cylinder-";
    fileName.append(boost::lexical_cast<std::string>(cylinderNum_));
    fileName += "-";
    fileName += currentTimeStr_;
    fileName += ".csv";
    filePrinter dataPrinter;
    dataPrinter.newFile(fileName);
    
    std::stringstream sstream;
    sstream << "Bout Range, Laps, Distance Covered(cm), Var.distCvrd, Avg Speed, Var.AvgSpd, Time Elapsed, Var.TimeElpsd\n";
    std::string str = sstream.str();
    sstream.str(std::string()); // clear the stringsteam
    dataPrinter.writeLine(str);
    
    unsigned int totalBouts = bouts_.size();

    
    std::string varianceAvgSpeed, varianceDistanceCvrd, varianceTimeElpsd;
    
    varianceDistanceCvrd = "=VAR(C2:C";
    varianceDistanceCvrd += boost::lexical_cast<std::string>(bouts_.size()+1);
    varianceDistanceCvrd += ")";
    
    varianceAvgSpeed = "=VAR(E2:E";
    varianceAvgSpeed += boost::lexical_cast<std::string>(bouts_.size()+1);
    varianceAvgSpeed += ")";

    varianceTimeElpsd = "=VAR(G2:G";
    varianceTimeElpsd += boost::lexical_cast<std::string>(bouts_.size()+1);
    varianceTimeElpsd += ")";
    
    
    sstream << bouts_[0].entryRange.first << ":" << bouts_[0].entryRange.second << ","
	    << bouts_[0].laps << ","
	    << bouts_[0].distanceCovered << "," << varianceDistanceCvrd << ","
	    << bouts_[0].avgSpeed << "," << varianceAvgSpeed << ","
	    << bouts_[0].timeElapsed << "," << varianceTimeElpsd << ",\n";
	str = sstream.str();
	sstream.str(std::string());
	dataPrinter.writeLine(str);
    for (unsigned int i = 1; i < totalBouts; i++)  
    {
	sstream << bouts_[i].entryRange.first << ":" << bouts_[i].entryRange.second << ","
		<< bouts_[i].laps << "," << bouts_[i].distanceCovered << ",,"
		<< bouts_[i].avgSpeed << ",,"
		<< bouts_[i].timeElapsed << ",," << "\n";
	str = sstream.str();
	sstream.str(std::string());
	dataPrinter.writeLine(str);

	
    } // end for
    
    
}


void dataEntry::printProcessedDataXLS()
{
    xlslib_core::workbook wb;
    xlslib_core::worksheet * sh = wb.sheet("main");
    
    sh->label(0,0,"Bout Range");
    sh->label(0,1,"Laps");
    sh->label(0,2,"Distance Covered(cm)");
    sh->colwidth(2,4700); // i think 3000 is the default size.
    sh->label(0,3,"Var.distCvrd");
    sh->colwidth(3,4000); // i think 3000 is the default size.
    sh->label(0,4,"Avg Speed");
    sh->label(0,5,"Var.AvgSpd");
    sh->colwidth(5,4000);
    sh->label(0,6,"Time Elapsed");
    sh->label(0,7,"Var.TimeElpsd");
    sh->colwidth(7,4000);
    unsigned int totalBouts = bouts_.size();

    std::string varianceAvgSpeed, varianceDistanceCvrd, varianceTimeElpsd;
    
    varianceDistanceCvrd = "=VAR(C2:C";
    varianceDistanceCvrd += boost::lexical_cast<std::string>(bouts_.size()+1);
    varianceDistanceCvrd += ")";
    
    varianceAvgSpeed = "=VAR(E2:E";
    varianceAvgSpeed += boost::lexical_cast<std::string>(bouts_.size()+1);
    varianceAvgSpeed += ")";

    varianceTimeElpsd = "=VAR(G2:G";
    varianceTimeElpsd += boost::lexical_cast<std::string>(bouts_.size()+1);
    varianceTimeElpsd += ")";  

    
    std::string entryRange;

    for (unsigned int i = 0; i < bouts_.size(); i++)
    {
	entryRange = boost::lexical_cast<std::string>(bouts_[i].entryRange.first);
	entryRange += ':';
	entryRange += boost::lexical_cast<std::string>(bouts_[i].entryRange.second);
	
	sh->label(i+1, 0, entryRange);
	
	sh->number(i+1, 1, bouts_[i].laps);
	
	sh->number(i+1, 2, bouts_[i].distanceCovered);
	
	sh->number(i+1, 4, bouts_[i].avgSpeed);

	sh->number(i+1, 6, bouts_[i].timeElapsed);
    }
    boost::filesystem::path currentPath("dumps");
    boost::filesystem::create_directory(currentPath);
    std::string fileName = currentPath.string();
    addFolderCharTo(fileName); // add '/' or '\\' depending on platform
    fileName.reserve(39); // expected size is 37-39
    fileName.append(boost::lexical_cast<std::string>(cylinderNum_));
    fileName += "-";
    fileName += currentTimeStr_;
    fileName += ".xls";
    wb.Dump(fileName);
}


double dataEntry::avgSpeedVaraince()
{
    int size(0);
    double mean(0.0);
    for (std::vector<boutEntry>::iterator it = bouts_.begin(); it != bouts_.end(); ++it) {
	mean += double(it->avgSpeed); 
	size++; // we save some time by not calling vec.size()
    }
    double temp = 0;
    for (int i = 0; i < size; i++)
    {
	temp += (mean-bouts_.at(i).avgSpeed *(mean-bouts_.at(i).avgSpeed));
	
    }
    return temp/size;
}

std::vector< boutEntry > dataEntry::getBoutEntriesList()
{
    // there has to be a better way to pass this data safely without copying.
    std::cerr << bouts_.size();
    return bouts_;
}



dataEntry::~dataEntry()
{
bouts_.clear();
}