#include <string>
#include <functional>
#include <SDL_opengl.h>
#include "../include/handle.h"
#include "../include/util.h"
#include "../include/gltexture.h"

const std::function<void(GLuint*)> GL::Texture::sTexDeleter = 
	[](GLuint *i){ glDeleteTextures(1, i); };

GL::Texture::Texture(){
}
GL::Texture::Texture(const std::string &file, bool load) : mFile(file) {
	if (load)
		Load(file);
}
void GL::Texture::Load(const std::string &file){
	mFile = file;
	mHandle = Handle(Util::LoadTexture(mFile), sTexDeleter);
}
void GL::Texture::Load(){
	mHandle = Handle(Util::LoadTexture(mFile), sTexDeleter);
}
void GL::Texture::Unload(){
	mHandle.Release();
}
std::string GL::Texture::File() const {
	return mFile;
}
GL::Texture::operator GLuint(){
	return mHandle;
}
