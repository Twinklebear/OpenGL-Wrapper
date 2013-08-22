#include <iostream>
#include <GL/glew.h>

#if defined(_MSC_VER)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "window.h"

oglw::SDL::SDL(){}
oglw::SDL::SDL(int flags){
	if (SDL_Init(flags) != 0)
		std::cout << "Failed to init!" << std::endl;
}
bool oglw::SDL::initSubSystem(int flags){
	return (SDL_InitSubSystem(flags) != 0);
}
oglw::SDL::~SDL(){
	SDL_Quit();
}

oglw::Window::Window(const std::string &title, int width, int height)
	: mWindow(nullptr)
{
	//Setup SDL_GL attributes we want, this must be done before opening window/making context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//Turn on multisample
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    //Create our window
	mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    //Make sure it created ok
    if (mWindow == nullptr)
        std::cout << "Window creation failed" << std::endl;

    //Get the glcontext
    mContext = SDL_GL_CreateContext(mWindow);
	//Make sure GLEW gets running alright
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK)
		std::cout << "GLEW init error" << std::endl;

	//Setup some properites for the context
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n"
        << "OpenGL Vendor: " << glGetString(GL_VENDOR) << "\n"
        << "OpenGL Renderer: " << glGetString(GL_RENDERER) << "\n"
        << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
oglw::Window::~Window(){
	close();
}
void oglw::Window::close(){
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}
void oglw::Window::clear(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void oglw::Window::present(){
	glFlush();
    SDL_GL_SwapWindow(mWindow);
}
