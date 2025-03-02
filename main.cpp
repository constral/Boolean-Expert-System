#include "files.h"
#include "cortex.h"
#include "speech.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>



WorkingMem WorkingMemory;
std::vector<std::string> memoryOutputVector;
std::vector<std::vector<std::vector<int> > > Pattern;



int main()
{
    // memory file is a query that has to be loaded
    loadMemoryFileTo(&memoryOutputVector);

    // load the induction patterns
    loadPatterns();

    // read the memory query
    interpretQuery(memoryOutputVector);

    // start user input program
    listen();
}
