#include "Replay.h"
#include <iostream>
#include <SDL\SDL.h>

using namespace ENGINE;

std::unique_ptr<Replay> Replay::replayMode(nullptr);

Replay::Replay(void)
{
	outStream = NULL;
	defaultLogFileName = "Settings.txt";
}

void ENGINE::Replay::PushForReplay(std::string str)
{
	replayButtons.push_back(str);
}

void ENGINE::Replay::SaveReplay(std::string filename)
{
	outStream = new std::ofstream(filename.c_str());
	for each (std::string msg in replayButtons) {
		(*outStream) << msg << "\n";
		outStream->flush();
	}
	close();
	std::cout << "Created Replay File" << std::endl;
}

std::string ENGINE::Replay::PullFromReplay()
{
	std::string temp = replayButtons.at(iterator);
	iterator++;
	return temp;
}

bool ENGINE::Replay::TryLoadReplay(std::string filename) {
	if (replayButtons.empty()) {
		LoadReplay(filename);
		return true;
	}
	return false;
}


bool ENGINE::Replay::LoadReplay(std::string filename)
{
	replayButtons.clear();
	std::string STRING;
	std::ifstream infile;
	infile.open(filename);
	if (!infile) //if there is no file of that type

	{
		std::cout << "An error has occurred while opening the file" << std::endl;
		infile.close();
		return false;
	}
	else { //pull everything into settings
		while (!infile.eof()) // To get you all the lines.
		{
			getline(infile, STRING); // Saves the line in STRING.
			if (STRING[0] == '+') {
				//discard this line since its a comment
			}
			else {
				replayButtons.push_back(STRING); //pushes the line into the file
			}

		}
		std::cout << "Successfully parsed the file" << std::endl;

		infile.close();
	}
	return true;
}

Replay::~Replay(void)
{
	Reset();
}

Replay* Replay::GetInstance(void)
{
	if (replayMode.get() == nullptr)
	{
		replayMode.reset(new Replay);
	}
	return replayMode.get();
}

void Replay::close()
{
	if (outStream != NULL)
	{
		outStream->close();
		delete outStream;
		outStream = NULL;
	}
}

void Replay::Reset() {
	replayButtons.clear();
	close();
}