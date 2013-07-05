#ifndef GLBUFFER_H
#define GLBUFFER_H

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
		template<class T>
		Buffer(const T &data, USAGE usage) : mNvals(1) {
			GLuint buf;
			glGenBuffers(1, &buf);
			mHandle = Handle(buf, sDeleter);
			bufferData(data, usage);
		}
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
		template<class T>
		void bufferData(const T &data, USAGE usage){
			mNvals = 1;
			glBindBuffer(B, mHandle);
			glBufferData(B, sizeof(T), &data, usage);
		}
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
		* Replace some data within the buffer at some offset
		* @param data The data to write
		* @param offset Offset in buffer to write too, in bytes
		*/
		template<class T>
		void bufferSubData(const T &data, size_t offset){
			mNvals = 1;
			glBindBuffer(B, mHandle);
			glBufferSubData(B, offset, sizeof(T), &data);
		}
		template<class T, size_t N>
		void bufferSubData(const std::array<T, N> &data, size_t offset){
			mNvals = N;
			glBindBuffer(B, mHandle);
			glBufferSubData(B, offset, N * sizeof(T), &data[0]);
		}
		template<class T>
		void bufferSubData(const std::vector<T> &data, size_t offset){
			mNvals = data.size();
			glBindBuffer(B, mHandle);
			glBufferSubData(B, offset, data.size() * sizeof(T), &data[0]);
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
		operator GLuint() const {
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
	//I can't seem to get this to compile for some reason 
	//template<>
	//const std::function<void(GLuint*)> Buffer<BUFFER::TRANSFORM_FEEDBACK>::sDeleter = [](GLuint *b){
	//	glDeleteTransformFeedbacks(1, b);
	//};
	//To compare buffers we just see if their handles are the same, ie. same GLuint ref
	template<BUFFER B>
	bool operator<(const Buffer<B> &rhs, const Buffer<B> &lhs){
		return (static_cast<GLuint>(rhs) < static_cast<GLuint>(lhs));
	}
	//Some convenient typedefs for shorter names
	typedef Buffer<BUFFER::ARRAY> VertexBuffer;
	typedef Buffer<BUFFER::ATOMIC_COUNTER> AtomicCounterBuffer;
	typedef Buffer<BUFFER::COPY_READ> CopyReadBuffer;
	typedef Buffer<BUFFER::COPY_WRITE> CopyWriteBuffer;
	typedef Buffer<BUFFER::DRAW_INDIRECT> DrawIndirectBuffer;
	typedef Buffer<BUFFER::DISPATCH_INDIRECT> DispatchIndirectBuffer;
	typedef Buffer<BUFFER::ELEMENT_ARRAY> ElementBuffer;
	typedef Buffer<BUFFER::PIXEL_PACK> PixelPackBuffer;
	typedef Buffer<BUFFER::PIXEL_UNPACK> PixelUnpackBuffer;
	typedef Buffer<BUFFER::SHADER_STORAGE> ShaderStorageBuffer;
	typedef Buffer<BUFFER::TEXTURE_BUFFER> TextureBuffer;
	typedef Buffer<BUFFER::TRANSFORM_FEEDBACK> TransformFeedbackBuffer;
	typedef Buffer<BUFFER::UNIFORM> UniformBuffer;
}

#endif
