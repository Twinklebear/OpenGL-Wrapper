#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../include/glvertexarray.h"
#include "../include/glprogram.h"
#include "../include/material.h"
#include "../include/model.h"

Model::Model(const std::vector<glm::vec3> &verts, const std::vector<unsigned short> &indices)
	: mActiveMat(nullptr), mModel(glm::mat4())
{
	mVao.Reference(GL::VertexBuffer(verts), "vbo");
	mVao.ElementBuffer(indices);
	//What to set as the attribute pointers here? Should
	//I just do layout location stuff?
}
void Model::AddMaterial(const std::string &name, Material &mat){
	mMaterials[name] = mat;
}
void Model::UseProgram(GL::Program &prog){
	mProg = prog;
	//Setup attributes
	mVao.SetAttribPointer("vbo", mProg.GetAttribute("position"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 0);
	mVao.SetAttribPointer("vbo", mProg.GetAttribute("normal"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 
		(void*)sizeof(glm::vec3));
	mVao.SetAttribPointer("vbo", mProg.GetAttribute("texIn"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3),
		(void*)(sizeof(glm::vec3) * 2));

	//Setup uniforms
	mProg.UniformMat4x4("m", mModel);
	UpdateColors();
}
void Model::UseMaterial(const std::string &name, bool textured){
	std::map<std::string, Material>::iterator mat = mMaterials.find(name);
	if (mat == mMaterials.end()){
		//Throw runtime err?
		std::cout << "Invalid material name: " << name << std::endl;
		return;
	}
	if (mActiveMat != nullptr)
		mActiveMat->UnloadTextures();
	mActiveMat = &(mat->second);
	if (textured)
		mActiveMat->LoadTextures();
	UpdateColors();
}
void Model::Translate(const glm::vec3 &vect){
	mModel = glm::translate(vect) * mModel;
	mProg.UniformMat4x4("m", mModel);
}
void Model::Rotate(float angle, const glm::vec3 &axis){
	mModel = mModel * glm::rotate(angle, axis);
	mProg.UniformMat4x4("m", mModel);
}
void Model::Scale(const glm::vec3 &scale){
	//The order of operations with rotate scale may be an issue
}
void Model::Draw(bool textured){
	GL::UseProgram(mProg);
	if (textured){
		GL::ActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mActiveMat->mapKa);
		//GL::ActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, mActiveMat->mapKd);
		//GL::ActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, mActiveMat->mapKs);
	}
	GL::BindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, mVao.NumElements("elem"), GL_UNSIGNED_SHORT, 0);
}
void Model::UpdateColors(){
	//If a material is set, pass the info
	if (mActiveMat != nullptr){
		mProg.Uniform3fv("kA", mActiveMat->kA);
		mProg.Uniform3fv("kD", mActiveMat->kD);
		mProg.Uniform3fv("kS", mActiveMat->kS);
		mProg.Uniform1f("nS", mActiveMat->nS);
	}
}
