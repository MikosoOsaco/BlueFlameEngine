#pragma once
#pragma once
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

namespace ENGINE {

	class Parser {
	public:
		Parser();
		~Parser();
		void LoadFileIntoParser(std::string filepath, bool debug);
		void PrintFile();
		std::vector<std::string> ParseBlock(char delimiter, char comment);
		inline bool isEmpty() { return parsedFile.empty(); };
	protected:
	private:
		std::vector<std::string> parsedFile;
	};
}

#endif