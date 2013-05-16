#include <vector>
#include <string>
#include <functional>
#include <GL/glew.h>
#include "../include/handle.h"
#include "../include/glvertexbuffer.h"
#include "../include/glvertexarray.h"

const std::function<void(GLuint*)> GL::VertexArray::sDeleter = 
	[](GLuint *vao){ glDeleteVertexArrays(1, vao); };

GL::VertexArray::VertexArray(){
	//Generate VAO, associate with the buffer and assign it to the handle
	GLuint vao;
	glGenVertexArrays(1, &vao);
	mHandle = Handle(vao, sDeleter);
}
void GL::VertexArray::elementBuffer(std::vector<unsigned short> indices){
	//TODO: Am I not associating the element buffer correctly? The constructor
	//will bind the buffer so shouldn't the vao pick it up?
	glBindVertexArray(mHandle);
	mElems = Buffer<ELEMENT_ARRAY>(indices, STATIC_DRAW);
	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
		std::cout << "gl error #: " << err << std::endl;
}
size_t GL::VertexArray::numElements(const std::string &name){
	return mElems.numVals();
}
void GL::VertexArray::setAttribPointer(const std::string &name, GLint attrib, size_t size, GLenum type,
			bool normalized, size_t stride, void *offset)
{
	//Make sure it's a vbo we've stored
	std::map<std::string, Buffer<ARRAY>>::iterator vbo = mVbos.find(name);
	if (vbo == mVbos.end())
		throw std::runtime_error("Invalid vbo name: " + name);

	glBindVertexArray(mHandle);
	glBindBuffer(BUFFER::ARRAY, vbo->second);
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, size, type, normalized, stride, offset);
}
GL::VertexArray::operator GLuint(){
	return mHandle;
}
