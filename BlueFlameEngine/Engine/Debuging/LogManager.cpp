#include "LogManager.h"
#include <ctime>

using namespace ENGINE;

LogManager* LogManager::logInstance = NULL;

LogManager::LogManager(void)
{
	outStream = NULL;
	defaultLogFileName = "logfile.log";
	currentSeverity = LOG_ERROR;
}

LogManager::~LogManager(void)
{
	close();
}

LogManager& LogManager::getInstance(void)
{
	if (logInstance == NULL)
	{
		logInstance = new LogManager();
	}
	return *logInstance;
}

void LogManager::setLogFile(std::string &fileName)
{
	close();
	outStream = new std::ofstream(fileName.c_str());

	//production build code, resets the severity to ensure only errors get pushed
	//not used during testing

	//currentSeverity = LOG_ERROR; 
}

void LogManager::close()
{
	if (outStream != NULL)
	{
		outStream->close();
		delete outStream;
		outStream = NULL;
	}
}

void LogManager::log(LogSeverity severity, std::string msg)
{
	if (currentSeverity != LOG_NONE) {
		if (severity <= currentSeverity) {
			if (outStream == NULL)
			{
				setLogFile(defaultLogFileName);
			}
			
			//time block for timestamping log messages
			time_t rawtime;

			time(&rawtime);

			std::string str = ctime(&rawtime);
			//end block
			
			(*outStream) << str.substr(0, str.length() - 2) << " ||| " << msg << "\n";
			outStream->flush();
		}	
	}
	// && currentSeverity > LOG_NONE)
	//{
		
	//}
	
}