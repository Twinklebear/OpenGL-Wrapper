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
#include <glbuffer.h>
#include <glvertexarray.h>
#include <glshader.h>
#include <glprogram.h>
#include <gltexture.h>
#include <model.h>
#include <util.h>

//This function demos working usage of ubo for matrices
int uboWorking();

//Note that we build with console as the system so that we'll be able to see
//the debug output and have the window stay open after closing the program
int main(int argc, char** argv){
	return uboWorking();
}

const std::array<glm::vec4, 10> quad = {
	//Vertex positions
	glm::vec4(-1.0, -1.0, 0.0, 1.0),
	glm::vec4(1.0, -1.0, 0.0, 1.0),
	glm::vec4(-1.0, 1.0, 0.0, 1.0),
	glm::vec4(1.0, 1.0, 0.0, 1.0),
	//Vertex colors
	glm::vec4(1.0, 0.0, 0.0, 1.0),
	glm::vec4(0.0, 1.0, 0.0, 1.0),
	glm::vec4(0.0, 0.0, 1.0, 1.0),
	glm::vec4(1.0, 1.0, 0.0, 1.0),
	//Texture UVs, stored 2 to a vec4
	glm::vec4(0.0, 0.0, 1.0, 0.0),
	glm::vec4(0.0, 2.0, 1.0, 2.0),
};
const std::array<unsigned short, 6> quadElems = {
	0, 1, 2,
	1, 3, 2
};

int uboWorking(){
	try {
		Window::init();
	}
	catch (const std::runtime_error &e){
		std::cout << e.what() << std::endl;
	}
	Input::Init();
	Window window("Test", 640, 480);

	//Setup vbo & ebo
	GL::VertexBuffer vbo(quad, GL::USAGE::STATIC_DRAW);
	GL::ElementBuffer ebo(quadElems, GL::USAGE::STATIC_DRAW);

	Util::checkError("VBO & EBO Setup");

	GL::VertexArray vao;
	vao.elementBuffer(ebo);
	Util::checkError("VAO setup");

	//Setup program
	GL::VertexShader vShader("../res/basic.v.glsl");
	if (!vShader.status())
		std::cout << vShader.getLog() << std::endl;
	Util::checkError("Vert shader Setup");

	GL::FragmentShader fShader("../res/multitexture.f.glsl");
	if (!fShader.status())
		std::cout << fShader.getLog() << std::endl;
	Util::checkError("Frag shader Setup");

	GL::Program program(vShader, fShader);
	Util::checkError("Prog Setup");

	//Pass vertex pos into position attrib
	GLint posAttrib = program.getAttribute("position");
	if (posAttrib == -1)
		std::cout << "Invalid position attrib loc" << std::endl;
	
	GLint colAttrib = program.getAttribute("color");
	if (colAttrib == -1)
		std::cout << "Invalid color attrib loc" << std::endl;

	GLint uvAttrib = program.getAttribute("texUv");
	if (uvAttrib == -1)
		std::cout << "Invalid uv attrib loc" << std::endl;

	vao.setAttribPointer(vbo, posAttrib, 4, GL_FLOAT);
	vao.setAttribPointer(vbo, colAttrib, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec4) * 4));
	vao.setAttribPointer(vbo, uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec4) * 8));
	Util::checkError("Pos attrib Setup");

	//Set the projection and model matrices
	glm::mat4 proj = glm::perspective(60.0f, 640.f / 480.f, 0.1f, 100.0f);
	glm::mat4 model = glm::translate<float>(0, 0, -2.5) * glm::rotate<float>(45, glm::vec3(0, 0, 1));
	std::array<glm::mat4, 2> matrices = { proj, model };

	GL::UniformBuffer ubo(matrices, GL::USAGE::STATIC_DRAW);
	program.bindUniformBlock("Mat", ubo);
	Util::checkError("Proj buf Setup");

	//Creating the texture binds it to TEXTURE_2D so no need to bind again
	GL::Texture<GL::TEXTURE::T2D> texture("../res/map.png", true);
	GL::Texture<GL::TEXTURE::T2D> textureB("../res/strip.png", true);

	GL::Sampler sampler;
	sampler.parameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
	sampler.parameteri(GL_TEXTURE_WRAP_T, GL_CLAMP);
	sampler.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	sampler.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	texture.bind(GL_TEXTURE0, sampler);
	textureB.bind(GL_TEXTURE1);
	program.uniform1i("texA", 0);
	program.uniform1i("texB", 1);

	program.use();
	while (!Input::Quit()){
		Input::PollEvents();
		if (Input::KeyDown(SDL_SCANCODE_ESCAPE))
			Input::Quit(true);

		window.clear();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		
		window.present();
	}
	Window::quit();

	return 0;
}
