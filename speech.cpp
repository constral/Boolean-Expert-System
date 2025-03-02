#include "files.h"
#include "cortex.h"
#include "speech.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>



// user input function
std::vector<std::string> getUserInput() {

    std::vector<std::string> final_vector;

    std::string buffer_for_line;
    std::string buffer_for_word;

    // provide cin input(s)
    getline(std::cin, buffer_for_line);
    // ...there is one single line, so you only get one input

    for(int char_counter = 0; char_counter < buffer_for_line.size(); char_counter++)
    {
        // if a word has been delimited
        if(buffer_for_line[char_counter] == ' ')
        {
            // push the found word into the final_vector
            final_vector.push_back(buffer_for_word);

            // clean the buffer
            buffer_for_word = "";
        }
        else
        {
            // concatenate the letter to the word you're reading
            buffer_for_word = buffer_for_word + buffer_for_line[char_counter];
        }
    }

    // push the last word into the final_vector
    final_vector.push_back(buffer_for_word);

    return final_vector;
}



// main function that calls the actions based on input
void listen() {

	std::string input;

	std::cout << "Choose an operation" << std::endl << std::endl;

	// listen while not exiting
	while (input != "exit")
	{
		std::cout << std::endl;
		std::cout << "Choose an operation" << std::endl;
		std::cin >> input;

		// write a query and call interpret_query() on it
		if(input == "solve")
		{
			// the query to call interpret_query on
			std::vector<std::string> query_to_solve;

			// give cin to write query in
			std::cout << std::endl;
			std::cout << "Enter the query to solve:" << std::endl;
            std::cin.ignore();

			query_to_solve = getUserInput();

			// solve the query
			interpretQuery(query_to_solve);
			// std::cout << "<action completed cout>" << std::endl;
		}

		// write a query and call memorizeQuery() on it
		else if(input == "memorize")
		{
			// the query to call interpret_query on
			std::vector<std::string> query_to_memorize;

			// give cin to write query in
			std::cout << std::endl;
			std::cout << "Enter the query to memorize:" << std::endl;
            std::cin.ignore();

			query_to_memorize = getUserInput();

			// the queried data should be identical to the memory file's structuring,
			// so call this simple function that just writes everything down
			memorizeQuery(query_to_memorize);
			std::cout << std::endl;
			std::cout << "Memorized query." << std::endl << std::endl;
		}

		else if(input == "exit")
        {
            std::cout << std::endl;
            std::cout << "Exiting." << std::endl;
        }

		// if you're writing something else or gibberish
		else
		{
			std::cout << std::endl;
			std::cout << "Query not recognized" << std::endl;
		}
	}
}
