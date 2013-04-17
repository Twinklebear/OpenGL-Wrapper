//#include <stdexcept>
//#include <string>
//#include <iostream>
//#include <functional>
//#include <chrono>
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include <glm/glm.hpp>
//#include <glm/ext.hpp>
//#include "window.h"
//#include "input.h"
//#include "timer.h"
//#include "glcamera.h"
//#include "glfunctions.h"
//#include "glshader.h"
//#include "glprogram.h"
//#include "glvertexbuffer.h"
//#include "glvertexarray.h"
//#include "util.h"
//#include "gltexture.h"
//
////Need to have this undef if not building with subsys = windows
//#undef main
//int main(int argc, char** argv){
//	//Start our window
//	try {
//		Window::Init();
//	}
//	catch (const std::runtime_error &e){
//		std::cout << e.what() << std::endl;
//	}
//	Input::Init();
//
//	//Setup main window to read input
//	int w = 640, h = 480;
//	Window window("Main window", w, h);
//	//Perhaps in future wrap calls to Bind and check against
//	//the currently bound value there to prevent unecessary rebinding
//	//Setup vbo
//	//GL::VertexBuffer vbo(vboCube);
//	std::vector<glm::vec3> verts;
//	std::vector<unsigned short> indices;
//	//Time the loading
//	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
//	Util::LoadObj("../res/cube.obj", verts, indices);
//	std::cout << "Model load time: " 
//		<< std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() / 1000.0f
//		<< std::endl;
//	
//	//setup vbos
//	GL::VertexBuffer vbo(verts);
//	//Setup vao
//	GL::VertexArray vao;
//	vao.Reference(vbo, "vert");
//	vao.ElementBuffer(indices);
//
//	//Setup program
//	GL::Program prog("../res/shader.v.glsl", "../res/shader.f.glsl");
//	//Setup the attributes
//	vao.SetAttribPointer("vert", prog.GetAttribute("position"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 0);
//	vao.SetAttribPointer("vert", prog.GetAttribute("normal"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), 
//		(void*)sizeof(glm::vec3));
//	vao.SetAttribPointer("vert", prog.GetAttribute("texIn"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3),
//		(void*)(sizeof(glm::vec3) * 2));
//
//	//Load the texture
//	GL::Texture texture("../res/map.png");
//	
//	//This is a parallel to gluLookAt, see http://www.opengl.org/sdk/docs/man2/xhtml/gluLookAt.xml for
//	//information on what eye, center and up represent, provides enough info to figure out math/transformations
//	//needed to make a camera
//	//Create a camera slightly above the box looking down at it
//	GL::Camera camera(glm::vec3(0, 0, 1), glm::vec3(0, 0, -5), glm::vec3(0, 1, 0));
//	//glm::mat4 proj = glm::ortho<float>(-4.0f, 4.0f, -4.0f, 4.0f, 0.1f, 100.0f);
//	glm::mat4 proj = glm::perspective(60.0f, (float)w / (float)h, 0.1f, 100.0f);
//	glm::vec3 rotation(0.0f, 0.0f, 0.0f);
//	glm::vec3 position(0, 0, -5);
//	glm::mat4 model = glm::translate(position) * glm::rotate(rotation.y, glm::vec3(0, 1, 0)) 
//		* glm::rotate(rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(rotation.z, glm::vec3(0, 0, 1));
//	prog.UniformMat4x4("p", proj);
//	prog.UniformMat4x4("v", camera.View());
//	prog.UniformMat4x4("m", model);
//
//	//Make a light @ origin
//	glm::vec4 lightPos(0.0f, 0.0f, 0.0f, 1.0f);
//	prog.Uniform4fv("lightPos", lightPos);
//
//	//Track if model matrix was be updated
//	bool mUpdate = false;
//	//speed for moving things
//	float speed = 8.0f, rotateSpeed = 50.0f;
//	Timer delta;
//	delta.Start();
//	//For tracking if we want to quit
//	while (!Input::Quit()){
//		float dT = delta.Restart() / 1000.0f;
//		//Event Handling
//		Input::PollEvents();
//		if (Input::KeyDown(SDL_SCANCODE_ESCAPE))
//			Input::Quit(true);
//		//Cube Controls------------------
//		//Cube rotation
//		if (Input::KeyDown(SDL_SCANCODE_D)){
//			rotation.y += rotateSpeed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_A)){
//			rotation.y -= rotateSpeed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_W)){
//			rotation.x -= rotateSpeed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_S)){
//			rotation.x += rotateSpeed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_Q)){
//			rotation.z += rotateSpeed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_E)){
//			rotation.z -= rotateSpeed * dT;
//			mUpdate = true;
//		}
//		//Cube translation
//		if (Input::KeyDown(SDL_SCANCODE_UP)){
//			position.y += speed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_DOWN)){
//			position.y -= speed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_LEFT)){
//			position.x -= speed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_RIGHT)){
//			position.x += speed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_Z)){
//			position.z += speed * dT;
//			mUpdate = true;
//		}
//		if (Input::KeyDown(SDL_SCANCODE_X)){
//			position.z -= speed * dT;
//			mUpdate = true;
//		}
//		//Camera strafe Left/Right
//		if (Input::KeyDown(SDL_SCANCODE_H))
//			camera.Strafe(glm::vec3(-speed * dT, 0, 0));
//		if (Input::KeyDown(SDL_SCANCODE_L))
//			camera.Strafe(glm::vec3(speed * dT, 0, 0));
//		//Strafe forward/back
//		if (Input::KeyDown(SDL_SCANCODE_I))
//			camera.Strafe(glm::vec3(0, 0, -speed * dT));
//		if (Input::KeyDown(SDL_SCANCODE_M))
//			camera.Strafe(glm::vec3(0, 0, speed * dT));
//		//Camera yaw and pitch while holding LMB
//		if (Input::MouseDown(1) && Input::MouseMotion()){
//			SDL_MouseMotionEvent move = Input::GetMotion();
//			//We want to damp the motion some, so / 5
//			camera.Yaw(-move.xrel * rotateSpeed / 5.0f * dT);
//			camera.Pitch(-move.yrel * rotateSpeed / 5.0f * dT);
//		}
//		//Camera roll while holding RMB
//		if (Input::MouseDown(SDL_BUTTON_RIGHT) && Input::MouseMotion()){
//			SDL_MouseMotionEvent move = Input::GetMotion();
//			//We want to damp the motion some, so / 5
//			camera.Roll(-move.xrel * rotateSpeed / 5.0f * dT);
//			//Camera zoom in/out
//			camera.Zoom(move.yrel * speed / 5.0f * dT);
//		}
//		//Reset cube rotation & position
//		if (Input::KeyDown(SDL_SCANCODE_R)){
//			rotation = glm::vec3(0, 0, 0);
//			position = glm::vec3(0, 0, -5);
//			mUpdate = true;
//		}
//
//		//Update model matrix when needed
//		if (mUpdate){
//			model = glm::translate(position) * glm::rotate(rotation.y, glm::vec3(0, 1, 0))
//				* glm::rotate(rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(rotation.z, glm::vec3(0, 0, 1));
//			prog.UniformMat4x4("m", model);
//			mUpdate = false;
//		}
//		if (camera.Changed())
//			prog.UniformMat4x4("v", camera.View());
//		
//		//Rendering
//		window.Clear();
//		//window.DrawElements(vao, prog, GL_TRIANGLES, indices.size());
//		window.DrawElementsTextured(vao, prog, texture, GL_TRIANGLES, vao.NumElements("elem"));
//		window.Present();
//
//		//cap at 60fps
//		if (delta.Ticks() < 1000 / 60)
//			SDL_Delay(1000 / 60 - delta.Ticks());
//	}
//	window.Close();
//	Window::Quit();
//	
//	return 0;
//}
