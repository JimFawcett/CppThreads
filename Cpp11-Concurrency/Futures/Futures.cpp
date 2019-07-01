///////////////////////////////////////////////////////////////
// Futures.cpp - parallel computation                        //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////

#include <future>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "FileSystem.h"

//----< search file for text string >--------------------------------

std::string search(const std::string& file, const std::string& text)
{
  std::ostringstream temp;
  std::ifstream in(file);
  if(in.good())
  {
    temp << in.rdbuf();
    std::string& contents = temp.str();
    size_t pos = contents.find(text);
    if(pos != std::string::npos)
      return file;
  }
  return "";
}
//----< parallel search using futures >------------------------------

int main()
{
  std::cout << "\n  Parallel text search using async and future";
  std::cout << "\n =============================================\n";

  std::vector<std::string> files = FileSystem::Directory::getFiles(".", "*.h");
  std::vector<std::string> more = FileSystem::Directory::getFiles(".","*.cpp");
  for(auto file : more)
    files.push_back(file);

  std::string searchString = "main";
  //std::string searchString = "#include";
  std::cout << "\n  files containing the string \"" << searchString << "\"";

  /******************************************************************
    sequential search
   *****************************************************************/
    //for(auto file : files)
    //{
    //  std::string result = search(file, searchString);
    //  if (result != "")
    //    std::cout << "\n    " << file;
    //}
 
  /******************************************************************
     parallel search
     Note: can't push_back future since its copy is private
   *****************************************************************/
  std::vector<std::future<std::string>> futures(files.size());
  for(size_t i=0; i<files.size(); ++i)
    futures[i] = std::async(search, files[i], searchString);  // asynchronous

  std::cout << "\n  after searching " << files.size() << " files for string \"" << searchString << "\":";

  // sequential display
  for(size_t i=0; i<files.size(); ++i)
  {
    std::string out = futures[i].get();  // blocking call
    if(out != "")
      std::cout << "\n    " << out;
  }

  std::cout << "\n\n";
  return 0;
}