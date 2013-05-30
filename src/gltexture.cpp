#include <string>
#include <functional>
#include <GL/glew.h>
#include <SOIL.h>
#include "../include/handle.h"
#include "../include/util.h"
#include "../include/gltexture.h"

const std::function<void(GLuint*)> GL::Texture::sDeleter = 
	[](GLuint *t){ glDeleteTextures(1, t); };

GL::Texture::Texture(){}
GL::Texture::Texture(const std::string &file, bool upload, unsigned int flags) 
	: mFile(file), mFlags(flags)
{
	if (upload)
		load();
}
void GL::Texture::load(const std::string &file, unsigned int flags){
	mFile = file;
	mFlags = flags;
	load();
}
void GL::Texture::load(){
	GLuint t = SOIL_load_OGL_texture(mFile.c_str(), SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, mFlags);
	mHandle = Handle(t, sDeleter);
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
