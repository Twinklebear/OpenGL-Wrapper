#include <string>
#include <glm/glm.hpp>
#include "../include/gltexture.h"
#include "../include/material.h"

Material::Material(){}
Material::Material(std::string name, glm::vec3 kA, glm::vec3 kD, glm::vec3 kS, float nS)
	: name(name), kA(kA), kD(kD), kS(kS), nS(nS)
{}
Material::Material(std::string name, glm::vec3 kA, glm::vec3 kD, glm::vec3 kS, float nS,
	GL::Texture mapKa, GL::Texture mapKd, GL::Texture mapKs)
	: name(name), kA(kA), kD(kD), kS(kS), nS(nS), mapKa(mapKa), mapKd(mapKd), mapKs(mapKs)
{}
void Material::loadTextures(){
	mapKa.load();
	mapKd.load();
	mapKs.load();
}
void Material::unloadTextures(){
	mapKa.unload();
	mapKd.unload();
	mapKs.unload();
}
