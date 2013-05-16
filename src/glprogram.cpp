#include <string>
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../include/handle.h"
#include "../include/glshader.h"
#include "../include/glprogram.h"

const std::function<void(GLuint*)> GL::Program::sProgDelete = 
	[](GLuint *p){ glDeleteProgram(*p); };

GL::Program::Program(){
}
GL::Program::Program(std::string vertex, std::string frag){
    Shader<VERTEX> vertShader(vertex);
    Shader<FRAGMENT> fragShader(frag);
    //Make sure the shaders are ok
    if (!vertShader.status() || !fragShader.status()){
        std::cout << vertShader.getLog() << std::endl
            << fragShader.getLog() << std::endl;
    }
	link(vertShader, fragShader);
}
GL::Program::Program(Shader<VERTEX> &vert, Shader<FRAGMENT> &frag){
	link(vert, frag);
}
void GL::Program::link(Shader<VERTEX> &vert, Shader<FRAGMENT> &frag){
	mHandle = Handle(glCreateProgram(), sProgDelete);
	glAttachShader(mHandle, vert);
    glAttachShader(mHandle, frag);
    glLinkProgram(mHandle);
}
std::string GL::Program::getLog(){
    if (!status()){
        //Get the log length and then get the log
        GLint logLength;
        glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> log(logLength);
        glGetShaderInfoLog(mHandle, logLength, NULL, &log[0]);
        
        return "Program errors:\n" + std::string(log.begin(), log.end());
    }
    return "Success";
}
bool GL::Program::status(){
    GLint status;
    glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
    return (status == GL_TRUE);
}
void GL::Program::use(){
	glUseProgram(mHandle);
}
GLint GL::Program::getAttribute(std::string name){
    return glGetAttribLocation(mHandle, name.c_str());
}
void GL::Program::uniform1i(const std::string &attrib, int i){
	glUseProgram(mHandle);
	GLint attribLoc = glGetUniformLocation(mHandle, attrib.c_str());
	glUniform1i(attribLoc, i);
}
void GL::Program::uniform1f(const std::string &attrib, float f){
	glUseProgram(mHandle);
	GLint attribLoc = glGetUniformLocation(mHandle, attrib.c_str());
	glUniform1f(attribLoc, f);
}
void GL::Program::uniform3fv(const std::string &attrib, const glm::vec3 &vec){
	glUseProgram(mHandle);
	GLint attribLoc = glGetUniformLocation(mHandle, attrib.c_str());
	glUniform3fv(attribLoc, 1, glm::value_ptr(vec));
}
void GL::Program::uniform4fv(const std::string &attrib, const glm::vec4 &vec){
    glUseProgram(mHandle);
    GLint attribLoc = glGetUniformLocation(mHandle, attrib.c_str());
    glUniform4fv(attribLoc, 1, glm::value_ptr(vec));
}
void GL::Program::uniformMat4x4(const std::string &attrib, const glm::mat4 &matrix){
    glUseProgram(mHandle);
    GLint attribLoc = glGetUniformLocation(mHandle, attrib.c_str());
    glUniformMatrix4fv(attribLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}
GL::Program::operator GLuint(){
    return mHandle;
}
