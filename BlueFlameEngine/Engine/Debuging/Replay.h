#pragma once

#ifndef REPLAY_H
#define REPLAY_H

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

///they said it shouldn't be done


namespace ENGINE {

	class Replay
	{
	public:
		std::string defaultLogFileName;

	private:
		static std::unique_ptr<Replay> replayMode; //ivan why is everything a singleton //well it makes sense this time
		std::ofstream		*outStream;

		Replay(void);
		std::vector<std::string> replayButtons;
		int iterator = 0;

	public:

		/**
		* This class is a singleton and this method returns a reference to the sole
		* instance of the class.
		* @returns The only instance of the LogManager.
		*/
		static Replay* GetInstance();

		/**
		* Destructor to close the log file and clean up.
		*/
		~Replay(void);

		/**
		* This closes the current log file without opening a new one.
		*/
		void close();

		bool TryLoadReplay(std::string filename);
		bool LoadReplay(std::string filename);
		void SaveReplay(std::string filename);
		void PushForReplay(std::string str);
		std::string PullFromReplay();
		void Reset();
	};
}

#endif
