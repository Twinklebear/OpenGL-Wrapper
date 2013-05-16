#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H

#include <functional>
#include <array>
#include <vector>
#include <GL/glew.h>
#include "handle.h"

#include <iostream>

namespace GL {
	//All OpenGL buffer types
	enum BUFFER { ARRAY = GL_ARRAY_BUFFER, ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BUFFER,
		COPY_READ = GL_COPY_READ_BUFFER, COPY_WRITE = GL_COPY_WRITE_BUFFER,
		DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER, DISPATCH_INDIRECT = GL_DISPATCH_INDIRECT_BUFFER,
		ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER, PIXEL_PACK = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK = GL_PIXEL_UNPACK_BUFFER, SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER, TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK,
		UNIFORM = GL_UNIFORM_BUFFER
	};
	//Buffer usage types
	enum USAGE { STREAM_DRAW = GL_STREAM_DRAW, STREAM_READ = GL_STREAM_READ,
		STREAM_COPY = GL_STREAM_COPY, STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ, STATIC_COPY = GL_STATIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW, DYNAMIC_READ = GL_DYNAMIC_READ,
		DYNAMIC_COPY = GL_DYNAMIC_COPY
	};
	/**
	* Handles and simplifies interacting with OpenGL Buffers
	*/
	template<BUFFER B>
	class Buffer {
	public:
		Buffer() : mNvals(0) {};
		/**
		* Create a new buffer using the data stored in the array passed
		* @param data The array of data to be passed
		* @param usage The usage hint for the buffer
		*/
		template<class T, size_t N>
		Buffer(const std::array<T, N> &data, USAGE usage) : mNvals(N) {
			GLuint buf;
			glGenBuffers(1, &buf);
			mHandle = Handle(buf, sDeleter);
			bufferData(data, usage);
		}
		template<class T>
		Buffer(const std::vector<T> &data, USAGE usage) : mNvals(data.size()) {
			GLuint buf;
			glGenBuffers(1, &buf);
			mHandle = Handle(buf, sDeleter);
			bufferData(data, usage);
		}
		/**
		* Write some data to the buffer, creating a new data store
		* and erasing any previous data
		* @param data The data to write
		* @param usage The usage hint for the buffer
		*/
		template<class T, size_t N>
		void bufferData(const std::array<T, N> &data, USAGE usage){
			mNvals = N;
			glBindBuffer(B, mHandle);
			glBufferData(B, N * sizeof(T), &data[0], usage);
		}
		template<class T>
		void bufferData(const std::vector<T> &data, USAGE usage){
			mNvals = data.size();
			glBindBuffer(B, mHandle);
			glBufferData(B, data.size() * sizeof(T), &data[0], usage);
		}
		/**
		* Get the buffer type 
		* @return the buffer type
		*/
		BUFFER type(){
			return B;
		}
		/**
		* Get the number of values
		*/
		size_t numVals(){
			return mNvals;
		}
		/**
		* Implicitly convert to a GLuint if trying to use
		* the program as such
		*/
		operator GLuint(){
			return mHandle;
		}

	private:
		Handle mHandle;
		size_t mNvals;
		const static std::function<void(GLuint*)> sDeleter;
	};
	template<BUFFER B>
	const std::function<void(GLuint*)> Buffer<B>::sDeleter = [](GLuint *b){
		glDeleteBuffers(1, b);
	};
}

#endif
