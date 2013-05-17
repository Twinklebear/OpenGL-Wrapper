#include <string>
#include <functional>
#include <GL/glew.h>
#include "../include/handle.h"
#include "../include/util.h"
#include "../include/gltexture.h"

const std::function<void(GLuint*)> GL::Texture::sDeleter = 
	[](GLuint *t){ glDeleteTextures(1, t); };

GL::Texture::Texture(){}
GL::Texture::Texture(const std::string &file, bool upload) : mFile(file) {
	if (upload)
		load(file);
}
void GL::Texture::load(const std::string &file){
	mFile = file;
	mHandle = Handle(Util::loadTexture(mFile), sDeleter);
}
void GL::Texture::load(){
	mHandle = Handle(Util::loadTexture(mFile), sDeleter);
}
void GL::Texture::unload(){
	mHandle.Release();
}
std::string GL::Texture::file() const {
	return mFile;
}
GL::Texture::operator GLuint(){
	return mHandle;
}
