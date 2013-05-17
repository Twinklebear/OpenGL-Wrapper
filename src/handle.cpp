#include <functional>
#include <SDL_opengl.h>
#include "../include/handle.h"

GL::Handle::Handle() : mObj(nullptr) {
	mObj = std::shared_ptr<GLuint>(new GLuint(-1));
}
GL::Handle::Handle(GLuint obj, std::function<void(GLuint*)> del) 
	: mObj(nullptr)
{
	mObj = std::shared_ptr<GLuint>(new GLuint(obj), del);
}
void GL::Handle::Release(){
	if (mObj != nullptr)
		mObj.reset();
}
GL::Handle::operator GLuint() const {
    return *mObj.get();
}
