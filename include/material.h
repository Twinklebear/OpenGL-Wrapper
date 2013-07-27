#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>
#include "gltexture.h"

/**
* A simple class to describe an object's material as described
* in a .mtl file under newmtl
*/
struct Material {
	Material();
	/**
	* Construct a simple un-textured material
	* @param name The material name
	* @param kA Ambient color
	* @param kD Diffuse color
	* @param kS Specular color
	* @param nS Specular highlight exponent
	*/
	Material(std::string name, glm::vec3 kA, glm::vec3 kD, glm::vec3 kS, float nS);
	/**
	* Construct a texture material, no bump map or nS map support yet
	* @param name The material name
	* @param kA Ambient color
	* @param kD Diffuse color
	* @param kS Specular color
	* @param nS Specular highlight exponent
	* @param mapKa Ambient texture 
	* @param mapKd Diffuse texture
	* @param mapKs Specular texture
	* @param load If we should load the textures onto the GPU now
	*/
	Material(std::string name, glm::vec3 kA, glm::vec3 kD, glm::vec3 kS, float nS,
		GL::Texture<GL::TEXTURE::T2D> mapKa, GL::Texture<GL::TEXTURE::T2D> mapKd, GL::Texture<GL::TEXTURE::T2D> mapKs);
	///Load the texture files onto the GPU for usage
	void loadTextures();
	///Unload the textures
	void unloadTextures();

	std::string name;
	glm::vec3 kA, kD, kS;
	float nS;
	GL::Texture<GL::TEXTURE::T2D> mapKa, mapKd, mapKs;
};

#endif