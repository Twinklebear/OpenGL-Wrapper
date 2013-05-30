#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <iostream>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL.h>
#include <SOIL.h>
#include "../include/material.h"
#include "../include/util.h"

std::ostream& operator<<(std::ostream &os, const glm::vec2 &v){
	for (size_t i = 0; i < v.length(); ++i)
		os << v[i] << ", ";
	return os;
}
std::ostream& operator<<(std::ostream &os, const glm::vec3 &v){
	for (size_t i = 0; i < v.length(); ++i)
		os << v[i] << ", ";
	return os;
}
bool Util::checkError(std::string msg){
		GLint err = glGetError();
		if (err != GL_NO_ERROR){
			std::cout << msg << " error: " << std::hex << err << std::endl;
			return true;
		}
		return false;
	}
std::string Util::readFile(const std::string &file){
    std::string content = "";
    std::ifstream fileIn(file.c_str());
    if (fileIn.is_open()){
        content = std::string((std::istreambuf_iterator<char>(fileIn)),
            std::istreambuf_iterator<char>());
    }
    return content;
}
std::map<std::string, Material> Util::loadMaterials(const std::string &file){
	std::map<std::string, Material> mats;
	if (file.substr(file.length() - 3, 3) != "mtl"){
		std::cout << "substr: " << file.substr(file.length() - 4, 3) << std::endl;
		std::cout << "Only .mtl files are supported, passed: " << file << std::endl;
		return mats;
	}
	std::ifstream mtlFile(file);
	if (!mtlFile.is_open()){
		std::cout << "Failed to find mtl file: " << file << std::endl;
		return mats;
	}
	//For matching numbers
	std::regex matchNum("((\\+|-)?[0-9]+)((\\.[0-9]+)?)");

	///Run through the file until we encounter a material definition, "newmtl"
	std::string line;
	while (std::getline(mtlFile, line)){
		//When we encounter a material definition load the material and store it in the map
		//We stop reading the material when we encounter a blank line or another material defintion
		if (line.substr(0, 6) == "newmtl"){
			Material material;
			//read the name
			material.name = line.substr(line.find_first_of(' ') + 1);
			//Read in material properties and grab the textures and colors
			while (std::getline(mtlFile, line) && !line.empty() && line.substr(0, 6) != "newmtl"){
				if (line.substr(0, 2) == "Ka")
					material.kA = capture<glm::vec3>(line, matchNum);
				else if (line.substr(0, 2) == "Kd")
					material.kD = capture<glm::vec3>(line, matchNum);
				else if (line.substr(0, 2) == "Ks")
					material.kS = capture<glm::vec3>(line, matchNum);
				else if (line.substr(0, 2) == "Ns")
					material.nS = capture<float>(line, matchNum);
				//Read texture maps
				else if (line.substr(0, 3) == "map"){
					if (line.substr(4, 2) == "Ka")
						material.mapKa = GL::Texture("../res/" + line.substr(7), false);
					else if (line.substr(4, 2) == "Kd")
						material.mapKd = GL::Texture("../res/" + line.substr(7), false);
					else if (line.substr(4, 2) == "Ks")
						material.mapKs = GL::Texture("../res/" + line.substr(7), false);
				}
			}
			mats[material.name] = material;
		}
	}
	return mats;
}
