#ifndef _filePrinter_hpp_
#define _filePrinter_hpp_

#include <ostream>
#include <string>
#include <sstream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


#include <iostream>

class filePrinter {
public:
  filePrinter();
  ~filePrinter();

public:
  // sets the default file output string to fileName
  void newFile(std::string fileName);
  // sets the file name to boost posix current time as a string
  void newFile_from_date();
  // set the filename to the input + boost posix current time +.txt as a string
  void newFile_from_date(std::string extraInFront);
  void writeLine(std::string str);
  // checks if the file exists
  void close();
  
  static bool doesFileExist(std::string fileName);
  
private:
  std::string fileName_;
  std::ofstream writeStream_;
};


#endif