#include <vector>
#include <string>
#include <functional>
#include <GL/glew.h>
#include "../include/handle.h"
#include "../include/glbuffer.h"
#include "../include/glvertexarray.h"

const std::function<void(GLuint*)> GL::VertexArray::sDeleter = 
	[](GLuint *vao){ glDeleteVertexArrays(1, vao); };

GL::VertexArray::VertexArray(){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	mHandle = Handle(vao, sDeleter);
}
void GL::VertexArray::elementBuffer(const std::vector<unsigned short> &indices){
	glBindVertexArray(mHandle);
	mElemBuf = ElementBuffer(indices, USAGE::STATIC_DRAW);
}
void GL::VertexArray::elementBuffer(ElementBuffer &e){
	glBindVertexArray(mHandle);
	glBindBuffer(BUFFER::ELEMENT_ARRAY, e);
	mElemBuf = e;
}
size_t GL::VertexArray::numElements(){
	return mElemBuf.numVals();
}
void GL::VertexArray::setAttribPointer(VertexBuffer &b, GLint attrib, size_t size, GLenum type,
	bool normalized, size_t stride, void *offset)
{
	glBindVertexArray(mHandle);
	glBindBuffer(BUFFER::ARRAY, b);
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, size, type, normalized, stride, offset);
	//Keep a reference of the buffer so it won't be deleted before the vao
	mVbos.insert(b);
}
GL::VertexArray::operator GLuint(){
	return mHandle;
}
