#include <functional>
#include <SDL_opengl.h>
#include "../include/handle.h"

GL::Handle::Handle() : mObj(nullptr) {}
GL::Handle::Handle(GLuint obj, std::function<void(GLuint*)> del) 
	: mObj(new GLuint(obj), del)
{}
void GL::Handle::release(){
	mObj.reset();
}
GL::Handle::operator GLuint() const {
	if (mObj == nullptr)
		return -1;
    return *mObj.get();
}
