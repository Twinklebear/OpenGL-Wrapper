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
	glm::vec4(0.0, 1.0, 1.0, 1.0),
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
	GL::VertexArray vao;
	vao.elementBuffer(ebo);

	GL::Program program("../res/basic.v.glsl", "../res/basic.f.glsl");
	vao.setAttribPointer(vbo, program.getAttribute("position"), 4, GL_FLOAT);
	vao.setAttribPointer(vbo, program.getAttribute("color"), 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec4) * 4));
	vao.setAttribPointer(vbo, program.getAttribute("texUv"), 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(glm::vec4) * 8));

	glm::mat4 quadModel = glm::translate(0.f, -1.f, 0.f) * glm::rotate(-90.f, glm::vec3(1.f, 0.f, 0.f));
	program.uniformMat4x4("m", quadModel);

	//Creating the texture binds it to TEXTURE_2D so no need to bind again
	GL::Texture<GL::TEXTURE::T2D> texture("../res/map.png");
	texture.bind(GL_TEXTURE0);
	program.uniform1i("tex2D", 0);

	//Setup a simple cube model to draw
	GL::VertexArray cubeVAO;
	GL::VertexBuffer cubeVBO;
	Model::loadObj("../res/cube.obj", cubeVAO, cubeVBO);

	GL::Program cubeProg("../res/cube_simple.v.glsl", "../res/cube_simple.f.glsl");
	cubeVAO.setAttribPointer(cubeVBO, cubeProg.getAttribute("position"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3));

	glm::mat4 cubeModel = glm::translate(0.f, -0.5f, 0.f) * glm::rotate(45.f, glm::vec3(0.f, 1.f, 0.f)) * glm::scale(0.3f, 0.3f, 0.3f);
	cubeProg.uniformMat4x4("m", cubeModel);

	//Testing an idea
	GLuint quadGBind = program.getUniformBlockIndex("Globals");
	GLuint cubeGBind = cubeProg.getUniformBlockIndex("Globals");

	//It seems that uniform block indices are a global thing, similar to how TEXTURE0 and such work
	//How can I make this more general? I'd need to track how many UBOs were active or something
	//Set the projection and other globals into a shared UBO
	glm::mat4 proj = glm::perspective(60.0f, 640.f / 480.f, 0.1f, 100.0f)
		* glm::lookAt(glm::vec3(0.f, 1.f, 2.f), glm::vec3(0.f, -1.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
	GL::UniformBuffer globalUbo(proj, GL::USAGE::STATIC_DRAW);
	//The default point is 0, but this is here to illustrate the idea
	glUniformBlockBinding(program, quadGBind, 0);
	glUniformBlockBinding(cubeProg, cubeGBind, 0);
	glBindBufferBase(static_cast<GLenum>(GL::BUFFER::UNIFORM), 0, globalUbo);

	Util::checkError("cube setup");

	while (!Input::Quit()){
		Input::PollEvents();
		if (Input::KeyDown(SDL_SCANCODE_ESCAPE))
			Input::Quit(true);

		window.clear();

		program.use();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

		cubeProg.use();
		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, cubeVAO.numElements(), GL_UNSIGNED_SHORT, 0);		

		window.present();
	}
	Window::quit();

	return 0;
}
