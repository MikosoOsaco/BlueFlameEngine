#include "Settings.h"
#include <iostream>
#include <SDL\SDL.h>

using namespace ENGINE;

std::unique_ptr<Settings> Settings::gameSettings(nullptr);

Settings::Settings(void)
{
	outStream = NULL;
	defaultLogFileName = "Settings.txt";
}

Settings::~Settings(void)
{
	close();
}

Settings* Settings::getInstance(void)
{
	if (gameSettings.get() == nullptr)
	{
		gameSettings.reset(new Settings);
	}
	return gameSettings.get();
}

void Settings::loadSettings(std::string fileName)
{
	settingsFile.clear();
	std::string STRING;
	std::ifstream infile;
	infile.open(fileName);
	if (!infile) //if there is no file of that type
	{
		std::cout << "An error has occurred while opening the file" << std::endl;
		infile.close();
		//build default settings file
		restoreDefaultSettings(fileName);
	}
	else { //pull everything into settings
		while (!infile.eof()) // To get you all the lines.
		{
			getline(infile, STRING); // Saves the line in STRING.
			if (STRING[0] == '+') {
				//discard this line since its a comment
			}
			else {
				settingsFile.push_back(STRING); //pushes the line into the file
			}

		}
		std::cout << "Successfully parsed the file" << std::endl;

		infile.close();

		if (settingsFile.size() >= 12) { //make sure we have the correct amount of settings
			setSettingsFromFile();
		}
		else {
			std::cout << "An error occured while reading the parsed file" << std::endl;
			restoreDefaultSettings(fileName);
		}
	}
}

void Settings::close()
{
	if (outStream != NULL)
	{
		outStream->close();
		delete outStream;
		outStream = NULL;
	}
}

void Settings::restoreDefaultSettings(std::string &fileName) {
	std::vector<std::string> defaultSettings;

	defaultSettings.push_back("+Settings file for Blue Flame Engine");
	defaultSettings.push_back("++++++++++++++++++++++++++++++++++++");
	defaultSettings.push_back("+Fullscreen (1 for true, 0 for false)");
	defaultSettings.push_back(std::to_string(fullscreen));
	defaultSettings.push_back("+Splitscreen (1 for true, 0 for false)");
	defaultSettings.push_back(std::to_string(splitscreen));
	defaultSettings.push_back("+Networked Multiplayer. Must be enabled to start in networked mode.");
	defaultSettings.push_back("+(1 for true, 0 for false)");
	defaultSettings.push_back(std::to_string(networkedGame));
	defaultSettings.push_back("+Acting as the server? If false, you will need an IP Address to connect to.");
	defaultSettings.push_back("+(1 for true, 0 for false)");
	defaultSettings.push_back(std::to_string(isServer));
	defaultSettings.push_back("+IP Address you're trying to connect to when acting as client");
	defaultSettings.push_back(serverIPAddress);
	defaultSettings.push_back("+A spectotor mode server will send data to be parsed by a spectator mode client");
	defaultSettings.push_back("+Spectator Mode Active? (1 for true, 0 for false)");
	defaultSettings.push_back(std::to_string(spectatorMode));
	defaultSettings.push_back("+Game Resolution");
	defaultSettings.push_back(std::to_string(resolutionX));
	defaultSettings.push_back(std::to_string(resolutionY));
	defaultSettings.push_back("+Use Bloom effects? (1 for true, 0 for false)");
	defaultSettings.push_back(std::to_string(useBloom));
	defaultSettings.push_back("+Show FPS counter in game? (1 for true, 0 for false)");
	defaultSettings.push_back(std::to_string(showFPS));
	defaultSettings.push_back("+Resolution Scale (goes from 0 to 1)");
	defaultSettings.push_back(std::to_string(resolutionScale));
	defaultSettings.push_back("+Replay System Enabled? (1 for true, 0 for false)");
	defaultSettings.push_back(std::to_string(replaySystemEnabled));
	defaultSettings.push_back("++++++++++++++++++++++++++++++++++++");

	close();
	outStream = new std::ofstream(fileName.c_str());
	for each (std::string msg in defaultSettings) {
		(*outStream) << msg << "\n";
		outStream->flush();
	}
	close();
	std::cout << "Populated default settings file" << std::endl;
}

void Settings::setSettingsFromFile() {
	if (settingsFile.at(0) == "0") {
		fullscreen = false;
	}
	else {
		fullscreen = true;
	}

	if (settingsFile.at(1) == "0") {
		splitscreen = false;
	}
	else {
		splitscreen = true;
	}

	if (settingsFile.at(2) == "0") {
		networkedGame = false;
	}
	else {
		networkedGame = true;
		if (settingsFile.at(3) == "1") {
			isServer = true;
		}
		else {
			isServer = false;
			serverIPAddress = settingsFile.at(4);
		}
		if (settingsFile.at(5) == "1") {
			spectatorMode = true;
		}
		else {
			spectatorMode = false;
		}
	}

	int x = resolutionX;
	try
	{
		x = stoi(settingsFile.at(6));
	}
	catch (const std::exception&)
	{

	}

	if (x > 0) {
		resolutionX = x;
	}


	int y = resolutionY;
	try
	{
		y = stoi(settingsFile.at(7));
	}
	catch (const std::exception&)
	{

	}
	if (y > 0) {
		resolutionY = y;
	}

	if (settingsFile.at(8) == "0") {
		useBloom = false;
	}
	else {
		useBloom = true;
	}

	if (settingsFile.at(9) == "0") {
		showFPS = false;
	}
	else {
		showFPS = true;
	}

	double z = resolutionScale;
	try
	{
		z = stod(settingsFile.at(10));
	}
	catch (const std::exception&)
	{

	}
	if (z >= 0 && z <= 1) {
		resolutionScale = z;
	}

	if (settingsFile.at(11) == "0") {
		replaySystemEnabled = false;
	}
	else {
		replaySystemEnabled = true;
	}
}
