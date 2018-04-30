#include "Parser.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace ENGINE;

Parser::Parser() {

}

Parser::~Parser() {

}

//loads a file into the parsedFile string vector. will print out its intake in debug mode
void Parser::LoadFileIntoParser(std::string filepath, bool debug) {
	std::string STRING;
	std::ifstream infile;
	infile.open(filepath);
	if (!infile)
	{
		std::cout << "An error has occurred while opening the file" << std::endl;
	}
	else {
		while (!infile.eof()) // To get you all the lines.
		{
			getline(infile, STRING); // Saves the line in STRING.
			if (debug) {
				std::cout << STRING << std::endl; // Prints our STRING.
			}
			parsedFile.push_back(STRING); //pushes the line into the file
		}
		std::cout << "Successfully parsed the file" << std::endl;

		infile.close();
	}
}

//prints the current string vector
void Parser::PrintFile() {
	if (!parsedFile.empty()) {
		for each (std::string s in parsedFile) {
			std::cout << s << std::endl;
		}
		std::cout << "Successfully printed the parsed file" << std::endl;
	}
	else {
		std::cout << "ERROR: PARSER EMPTY" << std::endl;
	}

}

std::vector<std::string> Parser::ParseBlock(char delimiter, char comment) {
	std::vector<std::string> parsedBlock;
	int blockChunk = 0;
	for each (std::string s in parsedFile) {
		blockChunk++;
		if (s.at(0) == comment) {
			//ignore the comment
			//parsedFile.erase(parsedFile.begin());
			continue;
		}
		else if (s.at(0) == delimiter) {
			break;
		}
		parsedBlock.push_back(s);

	}
	//remove the parsed block
	parsedFile.erase(parsedFile.begin(), parsedFile.begin() + blockChunk);
	std::cout << "PRINTING PARSED BLOCK" << std::endl;
	//in later years we'll return it
	for each (std::string s in parsedBlock) {
		std::cout << s << std::endl;
	}
	return parsedBlock;
}
