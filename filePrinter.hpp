#ifndef _filePrinter_hpp_
#define _filePrinter_hpp_

#include <ostream>
#include <string>
#include <sstream>
#include <fstream>

#include <boost/lexical_cast.hpp>

#include <iostream>

class filePrinter {
public:
  filePrinter();
  ~filePrinter();

public:
  // sets the default file output string to fileName
  void newFile(std::string fileName);
  
  void writeLine(std::string str);
  // checks if the file exists
  void close();
  
  static bool doesFileExist(std::string fileName);
  
private:
  std::string fileName_;
  std::ofstream writeStream_;
};


#endif