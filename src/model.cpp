#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../include/glvertexarray.h"
#include "../include/glbuffer.h"
#include "../include/glprogram.h"
#include "../include/material.h"
#include "../include/util.h"
#include "../include/model.h"

Model::Model(const std::string &file) : mActiveMat(nullptr) {
	loadObj(file);
}
void Model::addMaterial(const std::string &name, Material &mat){
	mMaterials[name] = mat;
}
void Model::useProgram(GL::Program &prog){
	mProg = prog;
	//Setup attributes
	mVao.setAttribPointer(mVbo, mProg.getAttribute("position"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 0);
	//mVao.setAttribPointer(mVbo, mProg.getAttribute("normal"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 
	//	(void*)sizeof(glm::vec3));
	//mVao.setAttribPointer(mVbo, mProg.getAttribute("texIn"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3),
	//	(void*)(sizeof(glm::vec3) * 2));

	//Setup uniforms
	//mProg.uniformMat4x4("m", mModel);
	updateColors();
}
void Model::useMaterial(const std::string &name){
	std::map<std::string, Material>::iterator mat = mMaterials.find(name);
	if (mat == mMaterials.end()){
		//Throw runtime err?
		std::cout << "Invalid material name: " << name << std::endl;
		return;
	}
	if (mActiveMat != nullptr)
		mActiveMat->unloadTextures();
	mActiveMat = &(mat->second);
	//Need to setup color uniforms if not using a texture
	//Actually we'd need to use a whole different shader too huh?
	//if (!mActiveMat->mapKa.file().empty())
	//	mActiveMat->loadTextures();
	//Need to be smarter about how the model stuff works, this isn't right
	//if the shader has no color inputs
	//UpdateColors();
}
void Model::translate(const glm::vec3 &vect){
	mModel = glm::translate(vect) * mModel;
	mProg.uniformMat4x4("m", mModel);
}
void Model::rotate(float angle, const glm::vec3 &axis){
	mModel = mModel * glm::rotate(angle, axis);
	mProg.uniformMat4x4("m", mModel);
}
void Model::scale(const glm::vec3 &scale){
	//The order of operations with rotate scale may be an issue
}
void Model::draw(bool textured){
	glUseProgram(mProg);
	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, mVao.numElements(), GL_UNSIGNED_SHORT, 0);
}
void Model::updateColors(){
	//If a material is set, pass the info
	if (mActiveMat != nullptr){
		mProg.uniform3fv("kA", mActiveMat->kA);
		mProg.uniform3fv("kD", mActiveMat->kD);
		mProg.uniform3fv("kS", mActiveMat->kS);
		mProg.uniform1f("nS", mActiveMat->nS);
	}
}
void Model::loadObj(const std::string &file){
	if (file.substr(file.length() - 3, 3) != "obj"){
		std::cout << "substr: " << file.substr(file.length() - 4, 3) << std::endl;
		std::cout << "Only .obj files are supported, passed: " << file << std::endl;
		return;
	}
	std::ifstream objFile(file);
	if (!objFile.is_open()){
		std::cout << "Failed to find obj file: " << file << std::endl;
		return;
	}
	//Matches a float value ie. of form xx.xx
	std::regex matchNum("((\\+|-)?[0-9]+)((\\.[0-9]+)?)");
	//Matches a face triangle vertex info
	std::regex matchVert("([0-9]+)/([0-9]+)/([0-9]+)");

	/*
	* The method for doing this will be to parse in all the vertex
	* positions, texture coords and normals then use the face information
	* to generate unique vertices with the v/vt/vn information when needed
	* and simultaneously generate the index buffer to point to the face vertices
	* ie. if we encounter a vertex we've already stored, 5/1/1 & we hit 5/1/1
	* again, instead of saving another vertex we add the existing index to the buffer
	* TODO: Handle simple mtl files. I think this will require creating a Model class of some
	* sort since now we're going to be dealing with a lot more information,
	* ie. vertices + indices + textures + ambient/diffuse/spec info and etc.
	*/
	//Store the vertex information as we read it in
	std::vector<glm::vec3> tempVert, tempNorm;
	std::vector<glm::vec2> tempTex;
	std::map<std::string, unsigned short> vertIndices;
	//The final vertex/index info
	std::vector<glm::vec3> verts;
	std::vector<unsigned short> indices;

	//Read the file and apply appropriate regexes to lines to get data
	std::string line, useMtl;
	while (std::getline(objFile, line)){
		//Skip empty lines
		if (line.empty())
			continue;
		//Parse vertex info, points, texture coords and normals
		if (line.at(0) == 'v'){
			//Matching vertices
			if (line.at(1) == ' '){
				tempVert.push_back(Util::capture<glm::vec3>(line, matchNum));
			}
			//Match texture coords
			if (line.at(1) == 't'){
				tempTex.push_back(Util::capture<glm::vec2>(line, matchNum));
			}
			//Match texture coords
			if (line.at(1) == 'n')
				tempNorm.push_back(Util::capture<glm::vec3>(line, matchNum));
		}
		//Match faces
		else if (line.at(0) == 'f'){
			auto begin = std::sregex_iterator(line.begin(), line.end(), matchVert);
			auto end = std::sregex_iterator();
			//Check if the vertex is already stored, if it is push back the index,
			//if not store the vertex and push in the new index
			for (auto i = begin; i != end; ++i){
				auto fnd = vertIndices.find(i->str());
				if (fnd != vertIndices.end())
					indices.push_back(fnd->second);
				//If we don't find it we need to store the vertex info, add to the indices vector and 
				//associate the face with the index in the map
				else {
					std::vector<int> v;
					Util::capture(i->str(), matchNum, v);
					//Setup the interleaved vbo
					verts.push_back(tempVert[v[0] - 1]);
					verts.push_back(tempNorm[v[2] - 1]);
					verts.push_back(glm::vec3(tempTex[v[1] - 1], 0));
					//Store the new index
					//We need to divide the index by the number of items we're interleaving
					indices.push_back((verts.size() - 1) / 3);
					//Associate the face with the index
					vertIndices[i->str()] = indices.back();
				}
			}
		}
		//Load material
		else if (line.substr(0, 3) == "mtl"){
			//The mtllib is specified like: mtllib name.mtl, so we find the first space
			//then split string at that and take the second half
			size_t pos = line.find(' ');
			std::string mtl = line.substr(pos + 1);
			mMaterials = Util::loadMaterials("../res/" + mtl);
		}
		//If we've found the material we want to use
		else if (line.substr(0, 6) == "usemtl")
			useMtl = line.substr(line.find(' ') + 1);
	}
	mVbo = GL::VertexBuffer(verts, GL::USAGE::STATIC_DRAW);
	mVao.elementBuffer(GL::ElementBuffer(indices, GL::USAGE::STATIC_DRAW));
	useMaterial(useMtl);
}
//Defined at end of file b/c it won't be a permanent resident
void Model::loadObj(const std::string &file, GL::VertexArray &vao, GL::VertexBuffer &vbo){
		if (file.substr(file.length() - 3, 3) != "obj"){
		std::cout << "substr: " << file.substr(file.length() - 4, 3) << std::endl;
		std::cout << "Only .obj files are supported, passed: " << file << std::endl;
		return;
	}
	std::ifstream objFile(file);
	if (!objFile.is_open()){
		std::cout << "Failed to find obj file: " << file << std::endl;
		return;
	}
	//Matches a float value ie. of form xx.xx
	std::regex matchNum("((\\+|-)?[0-9]+)((\\.[0-9]+)?)");
	//Matches a face triangle vertex info
	std::regex matchVert("([0-9]+)/([0-9]+)/([0-9]+)");

	/*
	* The method for doing this will be to parse in all the vertex
	* positions, texture coords and normals then use the face information
	* to generate unique vertices with the v/vt/vn information when needed
	* and simultaneously generate the index buffer to point to the face vertices
	* ie. if we encounter a vertex we've already stored, 5/1/1 & we hit 5/1/1
	* again, instead of saving another vertex we add the existing index to the buffer
	* TODO: Handle simple mtl files. I think this will require creating a Model class of some
	* sort since now we're going to be dealing with a lot more information,
	* ie. vertices + indices + textures + ambient/diffuse/spec info and etc.
	*/
	//Store the vertex information as we read it in
	std::vector<glm::vec3> tempVert, tempNorm;
	std::vector<glm::vec2> tempTex;
	std::map<std::string, unsigned short> vertIndices;
	//The final vertex/index info
	std::vector<glm::vec3> verts;
	std::vector<unsigned short> indices;

	//Read the file and apply appropriate regexes to lines to get data
	std::string line, useMtl;
	while (std::getline(objFile, line)){
		//Skip empty lines
		if (line.empty())
			continue;
		//Parse vertex info, points, texture coords and normals
		if (line.at(0) == 'v'){
			//Matching vertices
			if (line.at(1) == ' '){
				tempVert.push_back(Util::capture<glm::vec3>(line, matchNum));
			}
			//Match texture coords
			if (line.at(1) == 't'){
				tempTex.push_back(Util::capture<glm::vec2>(line, matchNum));
			}
			//Match texture coords
			if (line.at(1) == 'n')
				tempNorm.push_back(Util::capture<glm::vec3>(line, matchNum));
		}
		//Match faces
		else if (line.at(0) == 'f'){
			auto begin = std::sregex_iterator(line.begin(), line.end(), matchVert);
			auto end = std::sregex_iterator();
			//Check if the vertex is already stored, if it is push back the index,
			//if not store the vertex and push in the new index
			for (auto i = begin; i != end; ++i){
				auto fnd = vertIndices.find(i->str());
				if (fnd != vertIndices.end())
					indices.push_back(fnd->second);
				//If we don't find it we need to store the vertex info, add to the indices vector and 
				//associate the face with the index in the map
				else {
					std::vector<int> v;
					Util::capture(i->str(), matchNum, v);
					//Setup the interleaved vbo
					verts.push_back(tempVert[v[0] - 1]);
					verts.push_back(tempNorm[v[2] - 1]);
					verts.push_back(glm::vec3(tempTex[v[1] - 1], 0));
					//Store the new index
					//We need to divide the index by the number of items we're interleaving
					indices.push_back((verts.size() - 1) / 3);
					//Associate the face with the index
					vertIndices[i->str()] = indices.back();
				}
			}
		}
	}
	vbo = GL::VertexBuffer(verts, GL::USAGE::STATIC_DRAW);
	vao.elementBuffer(indices);
}
