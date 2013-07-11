#include <string>
#include <stdexcept>
#include <memory>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_opengl.h>
#include "../include/window.h"
#ifdef _DEBUG
#include <sstream>
#include "../include/logger.h"
#endif
#include <iostream>

#ifdef _DEBUG
/**
* OpenGL debug callback for logging opengl messages, the user param should be
* a Util::Logger*
*/
void APIENTRY glDebugCallback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar *msg, void *userParam);
#endif

Window::Window(std::string title, int width, int height)
	: mWindow(nullptr, SDL_DestroyWindow)
	//This looks pretty nasty but b/c no = operator for ostream we've got to construct the object here
#ifdef _DEBUG
	,debugOut(std::cout)
#endif
{
    //Setup our window
    mBox.x = 0;
    mBox.y = 0;
    mBox.w = width;
    mBox.h = height;

	//Setup SDL_GL attributes we want, this must be done before opening window/making context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Turn on multisample
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    //Create our window
    mWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        mBox.w, mBox.h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL));
    //Make sure it created ok
    if (mWindow == nullptr)
        throw std::runtime_error("Failed to create window");

    //Get the glcontext
    mContext = SDL_GL_CreateContext(mWindow.get());
	//Make sure GLEW gets running alright
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK){
#ifdef _DEBUG
		debugOut << "GLEW init error: " << glewGetErrorString(glewErr) << std::endl;
#endif
		throw std::runtime_error("GLEW init error");
	}
	//Setup some properites for the context
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glDebugCallback, &debugOut);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
    debugOut << "OpenGL Version: " << glGetString(GL_VERSION) << "\n"
        << "OpenGL Vendor: " << glGetString(GL_VENDOR) << "\n"
        << "OpenGL Renderer: " << glGetString(GL_RENDERER) << "\n"
        << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}
Window::~Window(){
	close();
}
void Window::init(){
    //initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		throw std::runtime_error("SDL Init Failed");
}
void Window::quit(){
    SDL_Quit();
}
//void Window::Draw(GL::VertexArray &vao, GL::Program &p, GLenum mode, int first, size_t count){
//    glUseProgram(p);
//    glBindVertexArray(vao);
//    glDrawArrays(mode, first, count);
//}
//void Window::DrawElements(GL::VertexArray &vao, GL::Program &p, GLenum mode, int count, size_t offset){
//	glUseProgram(p);
//	glBindVertexArray(vao);
//	glDrawElements(mode, count, GL_UNSIGNED_SHORT, (void*)offset);
//}
//void Window::DrawElementsTextured(GL::VertexArray &vao, GL::Program &p, GL::Texture &tex, GLenum mode, int count, size_t offset){
//	glUseProgram(p);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	glBindVertexArray(vao);
//	glDrawElements(mode, count, GL_UNSIGNED_SHORT, (void*)offset);
//}
void Window::clear(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::present(){
	glFlush();
    SDL_GL_SwapWindow(mWindow.get());
}
SDL_Rect Window::box(){
    return mBox;
}
bool Window::makeCurrent(){
	return (SDL_GL_MakeCurrent(mWindow.get(), mContext) == 0);
}
void Window::close(){
	SDL_GL_DeleteContext(mContext);
	mWindow.~unique_ptr();
}
#ifdef _DEBUG
void APIENTRY glDebugCallback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar *msg, void *userParam)
{
	Util::Logger &log = *static_cast<Util::Logger*>(userParam);
	log << "---OpenGL Message---\nTime: ";
	log.timeStamp();
	
	log << "Type: ";
	switch (type){
	case GL_DEBUG_TYPE_ERROR:
		log << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		log << "DEPRECATED BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		log << "UNDEFINED BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		log << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		log << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_MARKER:
		log << "MARKER";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		log << "PUSH GROUP";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		log << "POP GROUP";
		break;
	case GL_DEBUG_TYPE_OTHER:
		log << "OTHER";
		break;
	default:
		log << "UNKNOWN";
		break;
	}
	
	log << "\nFrom: ";
	switch (src){
	case GL_DEBUG_SOURCE_API:
		log << "API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		log << "WINDOW SYSTEM";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		log << "SHADER COMPILER";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		log << "THIRD PARTY";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		log << "APPLICATION";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		log << "OTHER";
		break;
	default:
		log << "UNKNOWN";
		break;
	}

	log << "\nSeverity: ";
	switch (severity){
	case GL_DEBUG_SEVERITY_LOW:
		log << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		log << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		log << "HIGH";
		break;
	default:
		log << "UNKNOWN";
		break;
	}

	log << "\nMessage: " << msg
		<< "\n---End GL Message---\n";
}
#endif
