#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <ostream>
#include <fstream>

namespace Util {
	/**
	* A simple logger class, will write messages sent to it
	* to the desired output (a file or any ostream)
	*/
	class Logger {
	public:
		/**
		* Assign the Logger a file to output too
		* @param file The filename of the log file (will be overwritten!)
		*/
		Logger(const std::string &file);
		/**
		* Assign the Logger an ostream to output to. User should make sure
		* this output stream remains open for the lifetime of the logger
		* @param os The ostream to write to
		*/
		Logger(std::ostream &os);
		/**
		* Write some string to the log file, the string will have the time since
		* the start of the program prepended to it, resulting in output like:
		* HH:MM:SS - msg
		* @param msg The message to log
		*/
		void log(const std::string &msg);
		//Write a timestamp in the log, a newline character is written after the stamp
		void timeStamp();
		//General stream output so the logger can behave like the output stream
		template<class T>
		Util::Logger& operator<<(const T &t){
			output << t;
			return *this;
		}
		//For handling std::endl and other modifiers
		Util::Logger& operator<<(std::ostream&(*f)(std::ostream&));

	private:
		//This will be used if the file constructor is called and we need to 
		//open our own file
		std::ofstream fileOut;
		//The output object we use so we can write to either os or the file
		std::ostream &output;
	};
}


#endif
