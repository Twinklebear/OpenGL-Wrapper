#include <stdexcept>
#include <string>
#include <iostream>
#include <functional>
#include <chrono>
#include <GL/glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <window.h>
#include <input.h>
#include <glvertexbuffer.h>
#include <glshader.h>
#include <glprogram.h>
#include <util.h>

//This function demos working usage of ubo for matrices
int uboWorking();

//Note that we build with console as the system so that we'll be able to see
//the debug output and have the window stay open after closing the program
int main(int argc, char** argv){
	return uboWorking();
}

const std::array<glm::vec4, 4> quad = {
	glm::vec4(-1.0, -1.0, 0.0, 1.0),
	glm::vec4(1.0, -1.0, 0.0, 1.0),
	glm::vec4(-1.0, 1.0, 0.0, 1.0),
	glm::vec4(1.0, 1.0, 0.0, 1.0)
};
const std::array<unsigned short, 6> quadElems = {
	0, 1, 2,
	1, 3, 2
};

int uboWorking(){
	try {
		Window::Init();
	}
	catch (const std::runtime_error &e){
		std::cout << e.what() << std::endl;
	}
	Input::Init();
	Window window("Test");

	//Setup vao
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	Util::CheckError("VAO Setup");
	
	//Setup vbo & ebo
	GL::Buffer<GL::BUFFER::ARRAY> vbo(quad, GL::USAGE::STATIC_DRAW);
	GL::Buffer<GL::BUFFER::ELEMENT_ARRAY> ebo(quadElems, GL::USAGE::STATIC_DRAW);

	Util::CheckError("VBO & EBO Setup");

	//Setup program
	GL::Shader<GL::SHADER::VERTEX> vShader("../res/basic.v.glsl");
	if (!vShader.status())
		std::cout << vShader.getLog() << std::endl;
	Util::CheckError("Vert shader Setup");

	GL::Shader<GL::SHADER::FRAGMENT> fShader("../res/basic.f.glsl");
	if (!fShader.status())
		std::cout << fShader.getLog() << std::endl;
	Util::CheckError("Frag shader Setup");

	GL::Program program(vShader, fShader);
	Util::CheckError("Prog Setup");

	//Pass vertex pos into position attrib
	GLint posAttrib = program.getAttribute("position");
	if (posAttrib == -1)
		std::cout << "Invalid position attrib loc" << std::endl;

	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	Util::CheckError("Pos attrib Setup");

	//Set the projection and model matrices
	glm::mat4 proj = glm::perspective(60.0f, (float)640 / (float)480, 0.1f, 100.0f);
	glm::mat4 model = glm::translate<float>(0, 0, -4) * glm::rotate<float>(45, glm::vec3(0, 0, 1));
	std::array<glm::mat4, 2> matrices = { proj, model };

	GLint projBufIdx = glGetUniformBlockIndex(program, "Mat");
	if (projBufIdx == GL_INVALID_INDEX)
		std::cout << "Invalid attribute location!" << std::endl;

	GL::Buffer<GL::BUFFER::UNIFORM> ubo(matrices, GL::USAGE::STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, projBufIdx, ubo);
	Util::CheckError("Proj buf Setup");

	program.use();
	while (!Input::Quit()){
		Input::PollEvents();
		if (Input::KeyDown(SDL_SCANCODE_ESCAPE))
			Input::Quit(true);

		window.Clear();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		
		window.Present();
	}
}
