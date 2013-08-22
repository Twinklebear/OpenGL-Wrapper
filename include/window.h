#ifndef OGLW_WINDOW_H
#define OGLW_WINDOW_H

#include <string>
#include <oglw_defines.h>

#if defined(_MSC_VER)
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

namespace oglw {
/**
* Small class for managing SDL, destructor will quit SDL
*/
class OGLW_API SDL {
public:
	//Empty constructor performs no intialization
	SDL();
	//Initialize SDL with desired flags
	SDL(int flags);
	//Initialize a desired subsytem
	bool initSubSystem(int flags);
	//Quit SDL
	~SDL();
};

/**
*  Window management class, provides a simple wrapper around
*  the SDL_Window and SDL_Renderer functionalities
*/
class OGLW_API Window {
public:
	/**
	* Create a new window and a corresponding gl context and make it current
	* @param title window title
	* @param width window width
	* @param height window height
	*/
	Window(const std::string &title, int width, int height);
	//Close the window
	~Window();
	//Close the window & delete the context
	void close();
	//Clear the renderer
    void clear();
    //Present the renderer, ie. update screen
    void present();

private:
	SDL_Window *mWindow;
	SDL_GLContext mContext;
};

}

#endif
