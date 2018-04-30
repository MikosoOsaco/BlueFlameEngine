#pragma once
//we need this definition to use the depricated ctime call.
//remove from final build once ctime is replaced
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <fstream>
#include <iomanip>
#include <string>

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

	class LogManager
	{
	public:
		//No logs, Critical errors, regular errors, warnings, traces, info, debug messages
		enum LogSeverity { LOG_NONE, LOG_CRIT, LOG_ERROR, LOG_WARN, LOG_TRACE, LOG_INFO, LOG_DEBUG };
		std::string defaultLogFileName;

	private:
		static LogManager	*logInstance; //there is only one LogManger++ instance, and it is an advanced instance
		std::ofstream		*outStream;
		LogSeverity			currentSeverity;

		LogManager(void);

	public:

		/**
		* This class is a singleton and this method returns a reference to the sole
		* instance of the class.
		* @returns The only instance of the LogManager.
		*/
		static LogManager& getInstance();

		/**
		* Destructor to close the log file and clean up.
		*/
		~LogManager(void);

		/**
		* THis closes any open log files and opens a new log file.
		* @param fileName The name of the new log file.
		*/
		void setLogFile(std::string &fileName);

		/**
		* This closes the current log file without opening a new one.
		*/
		void close();

		/**
		* This sets the severity of the LogManager.  Only log messages whose severity
		* is greater than or equal to the set severity will be output to the log file.
		* @param severity The minimum severity of messages to print.
		*/
		void setSeverity(LogSeverity severity)
		{
			currentSeverity = severity;
		}

		/**
		* Returns the current severity setting for the LogManager.
		* @returns the current severity level.
		*/
		LogSeverity getSeverity()
		{
			return currentSeverity;
		}

		/**
		* Logs a string with the indicated severity.  If the setting for severity
		* for the LogManager is less than this severity but not LOG_NONE, then the
		* message will be appended to the log file.
		* @param severity the severity of this log message
		* @param msg the message to be appended to the log file
		*/
		void log(LogSeverity severity, std::string msg);

		/**
		* Append a message to the log file with a severity of LOG_ERROR.
		* @param msg the message to append to the log file.
		*/
		void error(std::string msg)
		{
			std::string logthis = "ERROR: " + msg;
			log(LOG_ERROR, logthis);
		}

		/**
		* Append a message to the log file with a severity of LOG_WARN.
		* @param msg the message to append to the log file.
		*/
		void warn(std::string msg)
		{
			std::string logthis = "WARNING: " + msg;
			log(LOG_WARN, logthis);
		}

		/**
		* Append a message to the log file with a severity of LOG_TRACE.
		* @param msg the message to append to the log file.
		*/
		void trace(std::string msg)
		{
			std::string logthis = "TRACE: " + msg;
			log(LOG_TRACE, logthis);
		}

		/**
		* Append a message to the log file with a severity of LOG_INFO.
		* @param msg the message to append to the log file.
		*/
		void info(std::string msg)
		{
			std::string logthis = "INFO: " + msg;
			log(LOG_INFO, logthis);
		}

		/**
		* New warning types created by Ivan Boyko
		*/

		/**
		* Append a critical error message to the log file with a severity of LOG_CRIT.
		* @param msg the message to append to the log file.
		*/
		void critError(std::string msg)
		{
			std::string logthis = "CRITICAL ERROR: " + msg;
			log(LOG_CRIT, logthis);
		}

		/**
		* Append a debug message to the log file.
		* Debug messages have a severity of LOG_DEBUG and are not intended to be seen by end users.
		* @param msg the message to append to the log file.
		*/
		void debug(std::string msg)
		{
			std::string logthis = "DEBUG: " + msg;
			log(LOG_DEBUG, logthis);
		}

	};
}

#endif
