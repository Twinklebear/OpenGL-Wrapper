#include <GL/glew.h>
#include "../include/handle.h"
#include "../include/glsampler.h"

const std::function<void(GLuint*)> GL::Sampler::sDeleter = [](GLuint *s){
	glDeleteSamplers(1, s);
};

GL::Sampler::Sampler(){
	GLuint s;
	glGenSamplers(1, &s);
	mHandle = Handle(s, sDeleter);
}
void GL::Sampler::parameteri(GLenum pname, int param){
	glSamplerParameteri(static_cast<GLuint>(mHandle), pname, param);
}
void GL::Sampler::parameterf(GLenum pname, float param){
	glSamplerParameterf(static_cast<GLuint>(mHandle), pname, param);
}
GL::Sampler::operator GLuint(){
	return static_cast<GLuint>(mHandle);
}
