#include "fileRead.hpp"


fileRead::fileRead()
{
	lineNum_ = 1;
}

fileRead::fileRead(std::string fileName)
{
	setFileName(fileName);
}


void fileRead::setFileName(std::string filename)
{
	curFileName_ = filename;
}



std::string fileRead::ReadLineNext()
{
	if (curFileName_.empty()) {
	  return "ERROR: FileName doesn't exist or was not specefied";
	}
	
	streamBuffer_.open(curFileName_.c_str());
	
	std::getline(currentLine_, streamBuffer_ );
	 // if a windows return carraige character exists,  remove it to make the output portable to 
	 // to UNIX-like operating systems
 	size_t slashRHunter;
	if ((slashRHunter = currentLine_.find('\r')) != std::string::npos) {
		  currentLine_.erase(slashRHunter,  1); // erase the \r only
	}
	
	
  
  
}
		  


std::string fileRead::printCurrentString()
{
	return currentLine_;
}



void fileRead::reset()
{
	lineNum_ = 1;
	streamBuffer_.clear();
	if (streamBuffer_.is_open()) streamBuffer_.close();lineNum_ = 1;
	streamBuffer_.clear();
	if (streamBuffer_.is_open()) streamBuffer_.close();
}




long unsigned int fileRead::getLineNum()
{
	return lineNum_;
}



void fileRead::setLineNum(long unsigned int num)
{
  lineNum_ = num;
}


char fileRead::toLower(char character)
{
  if (character >= 65 && character <= 90) {
    character += 32;
  }
  return character;
}



fileRead::~fileRead()
{

}
