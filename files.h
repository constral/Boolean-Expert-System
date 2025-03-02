#ifndef FILES_H
#define FILES_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>



int StringToInteger(std::string s);

void loadMemoryFileTo(std::vector<std::string> *memoryOutputVector);

void memorizeQuery_loadMemoryFileTo(std::vector<std::string> *memorizeQueryOutputVector);

void loadPatterns();

void saveMemoryFileFrom(std::vector<std::string> data_text);

void memorizeQuery(std::vector<std::string> data_text);



extern std::vector<std::string> memoryOutputVector;
extern std::vector<std::vector<std::vector<int> > > Pattern;

#endif
