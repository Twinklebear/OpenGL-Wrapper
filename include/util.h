#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <sstream>
#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "material.h"

//Functions for reading in and printing out glm vectors, should be free of any namespace
//restrictions
//Debugging: add ability to print glm::vec2 and 3
std::ostream& operator<<(std::ostream &os, const glm::vec2 &v);
std::ostream& operator<<(std::ostream &os, const glm::vec3 &v);

namespace Util {
	//Check if there was an error and print it with some log message
	//return true if error
	bool checkError(std::string msg);
    //Read a file and return its contents as a string
	std::string readFile(const std::string &file);
	/**
	* Load the materials defined in a material lib file. Materials will be placed into
	* the map passed indexed by their name
	* @param file The mtl file to load from
	* @return The map of material name : material
	*/
	 std::map<std::string, Material> loadMaterials(const std::string &file);
	//Cast a string to a desired type and return it
	template<class T>
	T lexicalCast(const std::string &str){
		std::stringstream ss(str);
		T res;
		ss >> res;
		return res;
	}
	//Using scanf in our lexicalCast float/int gets us some more speed
	//Yea it's unsafe but at this point we're absolutely sure the type we want is
	//in there, so suppress the warning
#pragma warning(disable: 4996)
	template<>
	inline float lexicalCast(const std::string &str){
		float f;
		sscanf(str.c_str(), "%f", &f);
		return f;
		#pragma warning(default: 4996)
	}
#pragma warning(disable: 4996)
	template<>
	inline int lexicalCast(const std::string &str){
		int i;
		sscanf(str.c_str(), "%d", &i);
		return i;
		#pragma warning(default: 4996)
	}
	//Capture a single item
	template<class T>
	T capture(const std::string &str, const std::regex &reg){
		auto begin = std::sregex_iterator(str.begin(), str.end(), reg);
		auto end = std::sregex_iterator();
		T t;
		for (auto i = begin; i != end; ++i)
			t = lexicalCast<T>(i->str());
		return t;
	}
	//Capture a glm::vec3
	template<>
	inline glm::vec3 capture<glm::vec3>(const std::string &str, const std::regex &reg){
		auto begin = std::sregex_iterator(str.begin(), str.end(), reg);
		auto end = std::sregex_iterator();
		glm::vec3 v;
		size_t idx = 0;
		for (auto i = begin; i != end, idx < v.length(); ++i, ++idx)
			v[idx] = lexicalCast<float>(i->str());
		return v;
	}
	//Capture a glm::vec2
	template<>
	inline glm::vec2 capture<glm::vec2>(const std::string &str, const std::regex &reg){
		auto begin = std::sregex_iterator(str.begin(), str.end(), reg);
		auto end = std::sregex_iterator();
		glm::vec2 v;
		size_t idx = 0;
		for (auto i = begin; i != end, idx < v.length(); ++i, ++idx)
			v[idx] = lexicalCast<float>(i->str());
		return v;
	}
	//Capture matched values from regex results, values will be pushed onto vector
	template<class T>
	void capture(const std::string &str, const std::regex &reg, std::vector<T> &vect){
		auto begin = std::sregex_iterator(str.begin(), str.end(), reg);
		auto end = std::sregex_iterator();
		for (auto i = begin; i != end; ++i)
			vect.push_back(capture<T>(i->str(), reg));
	}
}

#endif