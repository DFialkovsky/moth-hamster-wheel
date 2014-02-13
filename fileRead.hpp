/* 
 * Daniel Fialkovsky 
 * 
 * MIT license 
 * 
 * File Readers merely takes two columns and reads them line by line
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */




#ifndef _fileRead_hpp_
#define _fileRead_hpp_


#include <fstream>
#include <ostream>
#include <string>
#include <boost/tokenizer.hpp>



class fileRead {
public:
	fileRead();
	fileRead(std::string fileName);
	~fileRead();
public:
	void setFileName(std::string filename);
	std::string ReadLineNext();
	std::string printCurrentString();
	unsigned long int getLineNum();
  
	void reset();
private:
	unsigned long int lineNum_;
	std::ifstream streamBuffer_;
	std::string currentLine_;
	std::string curFileName_;
  
private:
	void setLineNum(unsigned long int num);
	char toLower(char character);
};

#endif