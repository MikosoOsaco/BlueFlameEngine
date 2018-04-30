#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

/**
* LogManager++ provides an advanced logging service for an application that allows the application
* to write log messages to a file.  The Log Manager uses a severity system and the Log Manager
* can be configured to only print messages less than or equal to a specific severity.
* The LogManager is a singleton and the getInstance() method is used to access the
* sole instance of the LogManager.
* Orignal Author: R. N. Robson, HumDinger Engine
* Adapted for Engine++ by Ivan Boyko
*/
namespace ENGINE {

	class Settings
	{
	public:
		std::string defaultLogFileName;

	private:
		static std::unique_ptr<Settings> gameSettings; //ivan why is everything a singleton //well it makes sense this time
		std::ofstream		*outStream;

		Settings(void);
		std::vector<std::string> settingsFile;
		void restoreDefaultSettings(std::string &fileName);
		void setSettingsFromFile();
	public:

		/**
		* This class is a singleton and this method returns a reference to the sole
		* instance of the class.
		* @returns The only instance of the LogManager.
		*/
		static Settings* getInstance();

		/**
		* Destructor to close the log file and clean up.
		*/
		~Settings(void);

		/**
		* THis closes any open log files and opens a new settings file.
		* @param fileName The name of the new settings file.
		*/
		void loadSettings(std::string fileName);

		/**
		* This closes the current log file without opening a new one.
		*/
		void close();

		void PlayReplay() { playingReplay = true; }

		//settings

		bool fullscreen = false;
		bool splitscreen = true;
		bool networkedGame = false;
		bool isServer = true;
		std::string serverIPAddress = "";
		bool spectatorMode = false;
		int resolutionX = 800;
		int resolutionY = 640;
		bool useBloom = false;
		bool showFPS = true;
		double resolutionScale = 1.0;
		bool replaySystemEnabled = false;
		bool playingReplay = false;
	};
}

#endif
