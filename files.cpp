#include "files.h"
#include "cortex.h"
#include "speech.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>



// alternative to std::stoi()/std::atoi()
int StringToInteger(std::string s) {
    int i = 0;
    int k = 0;
    while(s[k] >= '0' && s[k] <= '9')
    {
        i = i * 10 + (s[k] - '0');
        k++;
    }
    return i;
}



//
// File loading functions
//

// outputs an interpretable query to a vector<string>
// skips loading of comments
void loadMemoryFileTo(std::vector<std::string> *memoryOutputVector) {

    // check if file exists
    FILE* pFileTwo;
    pFileTwo = fopen("memory.data", "r");
    if(pFileTwo == NULL)
    {
        std::cout << "ERROR: file 'memory.data' not found!" << std::endl;
    }

    // open the file
    std::ifstream read_memory_file("memory.data");
    std::string memory_file_string_buffer;

    bool comment_line_ended = 1;

    while(read_memory_file >> memory_file_string_buffer){

        if(memory_file_string_buffer == "/*" || comment_line_ended == 0)
        {
            comment_line_ended = 0;

            if(memory_file_string_buffer == "*/"){
                comment_line_ended = 1;
            }
        }
        else
        {
            memoryOutputVector->push_back(memory_file_string_buffer);
            //std::cout << memoryOutputVector.at(memoryOutputVector.size()-1) << std::endl;
        }
    }

    read_memory_file.close();

    /// optional error: "unable to load file 'memory.data'"
}

// special fork of loadMemoryFileTo() for use with memory save functions
// does not skip comments, skips reading the "nevig"
void memorizeQuery_loadMemoryFileTo(std::vector<std::string> *memorizeQueryOutputVector) {

    // check if file exists
    FILE* pFileTwo;
    pFileTwo = fopen("memory.data", "r");
    if(pFileTwo == NULL)
    {
        std::cout << "ERROR: file 'memory.data' not found!" << std::endl;
    }

    // open the file
    std::ifstream read_memory_file("memory.data");
    std::string memory_file_string_buffer;


    bool comment_line_ended = 1;

    // word by word loading method,
    // dunno about line by line
    while(read_memory_file >> memory_file_string_buffer){

        if(memory_file_string_buffer != "nevig"){

            memorizeQueryOutputVector->push_back(memory_file_string_buffer);
        }
    }

    read_memory_file.close();

    /// optional error: "unable to load file 'memory.data'"
}


// outputs a vector of defined patterns to "vector<vector<vector<int>>> Pattern"
void loadPatterns() {

	// 16 types of relations, but not all can be used in patterns
	// dimensions intialized with 17 rather than 0


	// on DIMENSION Z, two possible values for:
	/// variable_value
	std::vector<int> dimZ;
	dimZ.push_back(17);
	dimZ.push_back(17);

	// on DIMENSION Y, two possible values for:
	/// FtS_order
	std::vector<std::vector<int> > dimY;
	dimY.push_back(dimZ);
	dimY.push_back(dimZ);

	// on DIMENSION X, 16 possible values for:
	/// relation_type
	std::vector<std::vector<std::vector<int> > > dimX;
	for (int i = 0; i < 16; i++)
	{
		dimX.push_back(dimY);
	}

	// add all the stuff on dimX to the Pattern vector
	Pattern = dimX;

	/// MEANING THE FORMAT IS:
	// Pattern[relation_type][FtS_order][variable_value]
	// e.g. 12 1 1 0


	// now, to actually load result_values from them patterns

    std::vector<std::string> patternsOutputVector;

    // check if file exists
    FILE* pFileOne;
    pFileOne = fopen("patterns.data", "r");
    if(pFileOne == NULL)
    {
        std::cout << "ERROR: file 'patterns.data' not found!" << std::endl;
    }

    // open the file
    std::ifstream read_patterns_file("patterns.data");
    std::string patterns_file_string_buffer;

    bool comment_line_ended = 1;

    while(read_patterns_file >> patterns_file_string_buffer){

        if(patterns_file_string_buffer == "/*" || comment_line_ended == 0)
        {
            comment_line_ended = 0;

            if(patterns_file_string_buffer == "*/"){
                comment_line_ended = 1;
            }
        }
        else
        {
            patternsOutputVector.push_back(patterns_file_string_buffer);
        }
    }

    read_patterns_file.close();

    /*
    for(int xa = 0; xa < Pattern.size(); xa++){
        for(int ya = 0; ya < Pattern[xa].size(); ya++){
            for(int za = 0; za < Pattern[xa][ya].size(); za++){
                std::cout << Pattern[xa][ya][za] << " ";
            }
        }
        std::cout << " -- " << xa << std::endl;
    }
    */



    int patoutvecctr = 0;

    while(patoutvecctr < patternsOutputVector.size()){

    	// read the positions based on their format
    	// Pattern[relation_type][FtS_order][variable_value]
    	int pattern_relation_type = StringToInteger(patternsOutputVector[patoutvecctr]);
    	int pattern_FtS_order = StringToInteger(patternsOutputVector[patoutvecctr+1]);
    	int pattern_variable_value = StringToInteger(patternsOutputVector[patoutvecctr+2]);
    	int pattern_result = StringToInteger(patternsOutputVector[patoutvecctr+3]);

    	// add to array based on what you have read
    	Pattern[pattern_relation_type][pattern_FtS_order][pattern_variable_value] = pattern_result;

    	// increment to next pattern
    	patoutvecctr = patoutvecctr + 4;
    }

    /*
    for(int xa = 0; xa < Pattern.size(); xa++){
        for(int ya = 0; ya < Pattern[xa].size(); ya++){
            for(int za = 0; za < Pattern[xa][ya].size(); za++){
                std::cout << Pattern[xa][ya][za] << " ";;
            }
        }
        std::cout << " -- " << xa << std::endl;
    }
    */

    /// optional error: "unable to load file 'patterns.data'"
}





//
// File saving functions
//

// overwrites everything in the mem file
    /// including comments... so maybe you should actually use memorizeQuery() instead
// forked from interpret_query(vector<string> data_text)
// since both queries and the memory file have the same syntax, just write (correctly) whatever you find in the query
/// should make a backup before it starts overwriting!
void saveMemoryFileFrom(std::vector<std::string> data_text) {
/*
	// loop through the query's contents
	int string_looped_counter = 0;

	// open the memory file
    std::ofstream write_memory_file;

    // overwrites memory file
    /// this function creates a new file even if one doesn't exist
    write_memory_file.open("memory.data");

    /// add an error handler for a mistyped query

    // if there are still words to interpret in the query
	while(string_looped_counter != data_text.size()-1)
	{
		// when data is to be given
		if(data_text[string_looped_counter] == "given")
		{
			// advance to the next word
			string_looped_counter++;

			// loop through the given until you find its end
			while(data_text[string_looped_counter] != "nevig")
			{
				// when something new is declared
				if(data_text[string_looped_counter] == "-")
				{
					// it's a set!
					if(data_text[string_looped_counter+1] == "set")
					{
						// write a set format down
						write_memory_file << "- set " << data_text[string_looped_counter+2] << "\r\n";
					}

					// it's a relation!
					if(data_text[string_looped_counter+1] == "rel")
					{
						// write a rel format down
						write_memory_file << "- rel " << data_text[string_looped_counter+2] << " " << data_text[string_looped_counter+3] << " " << data_text[string_looped_counter+4] << " " << data_text[string_looped_counter+5] << "\r\n";
					}
				}

				// read the next thing
				string_looped_counter++;
			}

			// close the file once you're done with it
			write_memory_file.close();

			// the last item to be read will also have a linebreak inserted after it, but oh well
		}

		// that should be all to read inside a memorize query
	}
*/
}


/// should double-check with what is loaded in the memory to see if it's writing duplicate data
// adds to the data already written in the mem file
// forked from interpret_query(vector<string> data_text)
// since both queries and the memory file have the same syntax, just write (correctly) whatever you find in the query
void memorizeQuery(std::vector<std::string> data_text) {

    // loop through the query's contents
	int query_looped_counter = 0;
	int file_looped_counter = 0;

    std::vector<std::string> memorizeQueryOutputVector;

    // load everything inside the memory file
    memorizeQuery_loadMemoryFileTo(&memorizeQueryOutputVector);
    ///CONFIGURATION -- replace with loadMemoryFileTo(&memorizeQueryOutputVector) to skip saving comments
        /// has a bug with skipping the nevig

    // add everything inside the query to the vector
    // if there are still words to add
	while(query_looped_counter != data_text.size()-1)
	{
		// when data is to be given
		if(data_text[query_looped_counter] == "given")
		{
			// advance to the next word
			query_looped_counter++;

			// loop through the given until you find its end
			while(data_text[query_looped_counter] != "nevig")
			{
				// add absolutely everything from the query
				memorizeQueryOutputVector.push_back(data_text[query_looped_counter]);

				// read the next thing
				query_looped_counter++;

			}
		}
	}

	// append "nevig" at the end of the vector
    memorizeQueryOutputVector.push_back("nevig");
/*
    for(int xa = 0; xa < memorizeQueryOutputVector.size(); xa++){

        std::cout << memorizeQueryOutputVector.at(xa) << std::endl;
    }
*/


    // open the memory file
    std::ofstream write_memory_file;

    // overwrites memory file
    /// this function creates a new file even if one doesn't exist
    write_memory_file.open("memory.data");

    /// add an error handler for a mistyped query

    bool comment_line_ended = 1;

    // loop thru vector and overwrite everything in the memory
	while(file_looped_counter != memorizeQueryOutputVector.size())
	{
		if(memorizeQueryOutputVector.at(file_looped_counter) == "-" && comment_line_ended == 1)
		{
		    // it's a set!
            if(memorizeQueryOutputVector.at(file_looped_counter+1) == "set")
            {
                // appends "- " and the 2 words with linebreak
                write_memory_file << "\r\n" << "- set " << memorizeQueryOutputVector.at(file_looped_counter+2);

                file_looped_counter = file_looped_counter+2;
            }

            // it's a rel!
            if(memorizeQueryOutputVector.at(file_looped_counter+1) == "rel")
            {
                // appends "- " and the 4 words with linebreak
                write_memory_file << "\r\n" << "- rel " << memorizeQueryOutputVector.at(file_looped_counter+2) << " " << memorizeQueryOutputVector.at(file_looped_counter+3) << " " << memorizeQueryOutputVector.at(file_looped_counter+4) << " " << memorizeQueryOutputVector.at(file_looped_counter+5);

                file_looped_counter = file_looped_counter+4;
            }
		}

		// start of comments
		else if(memorizeQueryOutputVector.at(file_looped_counter) == "/*" || comment_line_ended == 0)
        {
            comment_line_ended = 0;

            // appends without linebreak
            write_memory_file << " " << memorizeQueryOutputVector.at(file_looped_counter);

            if(memorizeQueryOutputVector.at(file_looped_counter+1) == "*/"){

                comment_line_ended = 1;

                // appends "*/" with linebreak
                write_memory_file << " */";

                file_looped_counter++;
            }
        }

		// given
		else if(memorizeQueryOutputVector.at(file_looped_counter) == "given")
        {
            // appends a single word without linebreak
            write_memory_file << memorizeQueryOutputVector.at(file_looped_counter);
		}

		// nevig & other keywords
		else if(memorizeQueryOutputVector.at(file_looped_counter) == "nevig")
        {
            // appends a single word with linebreak
            write_memory_file << "\r\n" << memorizeQueryOutputVector.at(file_looped_counter);
		}

		file_looped_counter++;
	}

    // close the file once you're done with it
    write_memory_file.close();

}
