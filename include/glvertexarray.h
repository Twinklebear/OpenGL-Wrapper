#ifndef GLVERTEXARRAY_H
#define GLVERTEXARRAY_H

#include <set>
#include <functional>
#include <GL/glew.h>
#include "handle.h"
#include "glbuffer.h"

namespace GL {
	/**
	* Handles and simplifies interacting with OpenGL VBOs
	*/
	class VertexArray {
	public:
		/**
		* Create a VAO for use
		*/
		VertexArray();
		/**
		* Set the element buffer for indexed rendering, if desired
		* if the indices version is used a new element buffer will be made
		* if an existing element buffer is passed, it will be used
		*/
		template<size_t N>
		void elementBuffer(const std::array<unsigned short, N> &indices){
			glBindVertexArray(mHandle);
			mElemBuf = ElementBuffer(indices, USAGE::STATIC_DRAW);
		}
		void elementBuffer(const std::vector<unsigned short> &indices);
		void elementBuffer(ElementBuffer &e);
		//Get the number of elements in the element buffer
		size_t numElements();
		/**
		* Set some attribute to use values taken from a vbo given some
		* size of components to take, a type, if the values are normalize
		* a byte stride and an offset
		* @param b Vertex Buffer containing the data for this attribute
		* @param attrib The name of the attribute to set
		* @param vao The vertex array to get the values from
		* @param size The number of components per attribute
		* @param type The type of data in the vbo
		* @param normalized Specifies that fixed point values should be normalized if true, default false
		* @param stried The byte offset between attributes, default 0
		* @param offset The byte offset to begin reading attributes, default 0
		*/
		void setAttribPointer(VertexBuffer &b, GLint attrib, size_t size, GLenum type,
			bool normalized = GL_FALSE, size_t stride = 0, void *offset = 0);
		/**
		* Implicitly convert to a GLuint if trying to use
		* the program as such
		*/
		operator GLuint();

	private:
		///The VAO handle
		Handle mHandle;
		///The VBOs the VAO is referencing, we use a set to store two references to the same vbo
		std::set<VertexBuffer> mVbos;
		ElementBuffer mElemBuf;
		///A function to delete a single vao
		const static std::function<void(GLuint*)> sDeleter;
	};
}

#endif