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
	/*
	//Start our window
	try {
		Window::Init();
	}
	catch (const std::runtime_error &e){
		std::cout << e.what() << std::endl;
	}
	Input::Init();

	//Setup main window to read input
	int w = 640, h = 480;
	Window window("Main window", w, h);

	//Time the loading
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::shared_ptr<Model> model = Util::LoadObj("../res/suzanne.obj");
	std::cout << "Model load time: " 
		<< std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0f
		<< std::endl;

	//Setup program
	GL::Program prog("../res/modelshader.v.glsl", "../res/texturemodel.f.glsl");
	
	//This is a parallel to gluLookAt, see http://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml for
	//information on what eye, center and up represent, provides enough info to figure out math/transformations
	//needed to make a camera
	//Create a camera slightly above the box looking down at it
	GL::Camera camera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -5), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(60.0f, (float)w / (float)h, 0.1f, 100.0f);

	//Setup the Uniform buffer object
	GLuint uboIdx = glGetUniformBlockIndex(prog, "Lighting");
	if (uboIdx == GL_INVALID_INDEX)
		std::cout << "Invalid UBO Index" << std::endl;

	GLint uboSize;
	glGetActiveUniformBlockiv(prog, uboIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);

	//Make a light @ origin
	glm::vec4 lightPos(0.0f, 0.0f, 0.0f, 1.0f);
	//TODO: Generic Buffers, templated based on type maybe?? Also need to be able to allocate
	//arbitrary size buffer and write data to it instead of forced into passing vect/array/etc
	GL::VertexBuffer ubo(lightPos, GL::BUFFER::UNIFORM);
	glBindBufferBase(GL::BUFFER::UNIFORM, uboIdx, ubo);

	uboIdx = glGetUniformBlockIndex(prog, "VP");
	if (uboIdx == GL_INVALID_INDEX)
		std::cout << "Invalid UBO Index" << std::endl;
	std::cout << "VP ubo idx: " << uboIdx << std::endl;

	glGetActiveUniformBlockiv(prog, uboIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &uboSize);
	if (uboSize != sizeof(glm::mat4) * 2)
		std::cout << "ubo size isn't right!" << std::endl;

	std::cout << "Creating vp ubo" << std::endl;
	//std::array<glm::mat4, 2> matVP = { camera.View(), proj };
	std::vector<glm::mat4> matVP;
	matVP.push_back(camera.View());
	matVP.push_back(proj);
	GL::VertexBuffer matVPubo(matVP, GL::BUFFER::UNIFORM);
	glBindBufferBase(GL::BUFFER::UNIFORM, uboIdx, matVPubo);

	//Trying to figure out what's wrong with this on Nvidia
	const char *names[] = { "v", "p" };
	GLuint indices[2];
	glGetUniformIndices(prog, 2, names, indices);
	std::cout << "Indices: " << indices[0] << ", " << indices[1] << std::endl;

	//now query some info
	GLint info[2];
	glGetActiveUniformsiv(prog, 2, indices, GL_UNIFORM_OFFSET, info);
	std::cout << "Offsets: " << info[0] << ", " << info[1] << std::endl;

	glGetActiveUniformsiv(prog, 2, indices, GL_UNIFORM_MATRIX_STRIDE, info);
	std::cout << "Matrix strides: " << info[0] << ", " << info[1] << std::endl;

	model->UseProgram(prog);
	model->Translate(glm::vec3(0, 0, -5));

	//Track if model matrix was be updated
	bool mUpdate = false;
	//speed for moving things
	float speed = 8.0f, rotateSpeed = 50.0f;
	Timer delta;
	delta.Start();
	//For tracking if we want to quit
	while (!Input::Quit()){
		float dT = delta.Restart() / 1000.0f;
		//Event Handling
		Input::PollEvents();
		if (Input::KeyDown(SDL_SCANCODE_ESCAPE))
			Input::Quit(true);
		//Cube Controls------------------
		//Cube rotation
		if (Input::KeyDown(SDL_SCANCODE_D)){
			model->Rotate(rotateSpeed * dT, glm::vec3(0, 1, 0));
		}
		if (Input::KeyDown(SDL_SCANCODE_A)){
			model->Rotate(-rotateSpeed * dT, glm::vec3(0, 1, 0));
		}
		if (Input::KeyDown(SDL_SCANCODE_W)){
			model->Rotate(-rotateSpeed * dT, glm::vec3(1, 0, 0));
		}
		if (Input::KeyDown(SDL_SCANCODE_S)){
			model->Rotate(rotateSpeed * dT, glm::vec3(1, 0, 0));
		}
		if (Input::KeyDown(SDL_SCANCODE_Q)){
			model->Rotate(rotateSpeed * dT, glm::vec3(0, 0, 1));
		}
		if (Input::KeyDown(SDL_SCANCODE_E)){
			model->Rotate(-rotateSpeed * dT, glm::vec3(0, 0, 1));
		}
		//Cube translation
		if (Input::KeyDown(SDL_SCANCODE_UP)){
			model->Translate(glm::vec3(0, speed * dT, 0));
		}
		if (Input::KeyDown(SDL_SCANCODE_DOWN)){
			model->Translate(glm::vec3(0, -speed * dT, 0));
		}
		if (Input::KeyDown(SDL_SCANCODE_LEFT)){
			model->Translate(glm::vec3(-speed * dT, 0, 0));
		}
		if (Input::KeyDown(SDL_SCANCODE_RIGHT)){
			model->Translate(glm::vec3(speed * dT, 0, 0));
		}
		if (Input::KeyDown(SDL_SCANCODE_Z)){
			model->Translate(glm::vec3(0, 0, speed * dT));
		}
		if (Input::KeyDown(SDL_SCANCODE_X)){
			model->Translate(glm::vec3(0, 0, -speed * dT));
		}
		//Camera strafe Left/Right
		if (Input::KeyDown(SDL_SCANCODE_H))
			camera.Strafe(glm::vec3(-speed * dT, 0, 0));
		if (Input::KeyDown(SDL_SCANCODE_L))
			camera.Strafe(glm::vec3(speed * dT, 0, 0));
		//Strafe forward/back
		if (Input::KeyDown(SDL_SCANCODE_I))
			camera.Strafe(glm::vec3(0, 0, -speed * dT));
		if (Input::KeyDown(SDL_SCANCODE_M))
			camera.Strafe(glm::vec3(0, 0, speed * dT));
		//Camera yaw and pitch while holding LMB
		if (Input::MouseDown(1) && Input::MouseMotion()){
			SDL_MouseMotionEvent move = Input::GetMotion();
			//We want to damp the motion some, so / 5
			camera.Yaw(-move.xrel * rotateSpeed / 5.0f * dT);
			camera.Pitch(-move.yrel * rotateSpeed / 5.0f * dT);
		}
		//Camera roll while holding RMB
		if (Input::MouseDown(SDL_BUTTON_RIGHT) && Input::MouseMotion()){
			SDL_MouseMotionEvent move = Input::GetMotion();
			//We want to damp the motion some, so / 5
			camera.Roll(-move.xrel * rotateSpeed / 5.0f * dT);
			//Camera zoom in/out
			camera.Zoom(move.yrel * speed / 5.0f * dT);
		}
		if (camera.Changed())
			matVPubo.BufferSubData(camera.View(), 0);
		
		//Rendering
		window.Clear();
		model->Draw(true);
		window.Present();

		//cap at 60fps
		if (delta.Ticks() < 1000 / 60)
			SDL_Delay(1000 / 60 - delta.Ticks());
	}
	window.Close();
	Window::Quit();
	*/
	return 0;
}
const char *vShaderSrc = 
	"#version 330 \n \
	layout (std140) uniform Mat { mat4 p; mat4 m; }; \
	in vec4 position; \
	void main(){ gl_Position = p * m * position; } ";

const char *fShaderSrc =
	"#version 330 \n \
	out vec4 color; \
	void main(){ color = vec4(1.0f, 1.0f, 1.0f, 1.0f); } ";

//Don't want to bother with element buffers for this simple ex.
const std::array<glm::vec4, 3> tri = {
	glm::vec4(-1.0, -1.0, -2.0, 1.0),
	glm::vec4(1.0, -1.0, -2.0, 1.0),
	glm::vec4(-1.0, 1.0, -2.0, 1.0)
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

	//Setup vbo
	GL::Buffer<GL::BUFFER::ARRAY> vbo(tri, GL::USAGE::STATIC_DRAW);

	Util::CheckError("VBO Setup");
	//Setup vao
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	Util::CheckError("VAO Setup");

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
	glm::mat4 model = glm::translate<float>(0, 0, -1) * glm::rotate<float>(45, glm::vec3(0, 0, 1));
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

		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		window.Present();
	}
}
