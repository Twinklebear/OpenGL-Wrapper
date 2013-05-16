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
#include "model.h"

//Functions for reading in and printing out glm vectors, should be free of any namespace
//restrictions
//Debugging: add ability to print glm::vec2 and 3
std::ostream& operator<<(std::ostream &os, const glm::vec2 &v);
std::ostream& operator<<(std::ostream &os, const glm::vec3 &v);

namespace Util {
	//Check if there was an error and print it with some log message
	//return true if error
	bool CheckError(std::string msg);
    //Read a file and return its contents as a string
	std::string ReadFile(const std::string &file);
	/**
	* Load the object file passed and return the model, will also try to 
	* load and pick active material based on mtllib and usemtl commands in file
	* @param file File to load from
	* @return The loaded model
	*/
	//std::shared_ptr<Model> LoadObj(const std::string &file);
	/**
	* Load the materials defined in a material lib file. Materials will be placed into
	* the map passed indexed by their name
	* @param file The mtl file to load from
	* @return The map of material name : material
	*/
	 std::map<std::string, Material> LoadMaterials(const std::string &file);
	/**
	* Load a texture and return the handle
	* TODO: Add ability to set options for loading the texture
	* @param file File to load texture from
	* @return The GLuint associated with the texture
	*/
	GLuint LoadTexture(const std::string &file);
	//Cast a string to a desired type and return it
	template<class T>
	T lexicalCast(const std::string &str){
		std::stringstream ss;
		T res;
		ss << str;
		ss >> res;
		return res;
	}
	//Using scanf in our lexicalCast float/int gets us some more speed
	template<>
	inline float lexicalCast(const std::string &str){
		float f;
		sscanf(str.c_str(), "%f", &f);
		return f;
	}
	template<>
	inline int lexicalCast(const std::string &str){
		int i;
		sscanf(str.c_str(), "%d", &i);
		return i;
	}
	//Capture a single item
	template<class T>
	T capture(const std::string &str, const std::regex &reg){
		auto begin = std::sregex_iterator(str.begin(), str.end(), reg);
		auto end = std::sregex_iterator();
		T t;
		for (std::sregex_iterator i = begin; i != end; ++i)
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
		for (std::sregex_iterator i = begin; i != end, idx < v.length(); ++i, ++idx)
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
		for (std::sregex_iterator i = begin; i != end, idx < v.length(); ++i, ++idx)
			v[idx] = lexicalCast<float>(i->str());
		return v;
	}
	//Capture matched values from regex results, values will be pushed onto vector
	template<class T>
	void capture(const std::string &str, const std::regex &reg, std::vector<T> &vect){
		auto begin = std::sregex_iterator(str.begin(), str.end(), reg);
		auto end = std::sregex_iterator();
		for (std::sregex_iterator i = begin; i != end; ++i)
			vect.push_back(capture<T>(i->str(), reg));
	}
}

#endif