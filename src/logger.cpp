#include <string>
#include <ostream>
#include <fstream>
#include <sstream>
#include <SDL.h>
#include "../include/logger.h"

Util::Logger::Logger(const std::string &file) 
	: fileOut(file.c_str()), output(fileOut)
{
}
Util::Logger::Logger(std::ostream &os) : output(os) {
}
void Util::Logger::log(const std::string &msg){
	timeStamp();
	output << msg;
}
std::string Util::Logger::timeStamp(){
	float sec = SDL_GetTicks() / 1000.f;
	int hrs = sec / 3600;
	int min = sec / 60 - hrs * 3600;
	sec = static_cast<int>(sec - hrs * 3600 - min * 60);
	std::stringstream ss;
	ss << hrs << ":" << min << ":" << sec << "\n";
	return ss.str();
}
Util::Logger& Util::Logger::operator<<(std::ostream&(*f)(std::ostream&)){
	output << f;
	return *this;
}