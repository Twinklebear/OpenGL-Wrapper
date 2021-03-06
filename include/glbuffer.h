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
	enum class BUFFER { ARRAY = GL_ARRAY_BUFFER, ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BUFFER,
		COPY_READ = GL_COPY_READ_BUFFER, COPY_WRITE = GL_COPY_WRITE_BUFFER,
		DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER, DISPATCH_INDIRECT = GL_DISPATCH_INDIRECT_BUFFER,
		ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER, PIXEL_PACK = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK = GL_PIXEL_UNPACK_BUFFER, SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER, TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
		UNIFORM = GL_UNIFORM_BUFFER
	};
	//Buffer usage types
	enum class USAGE { STREAM_DRAW = GL_STREAM_DRAW, STREAM_READ = GL_STREAM_READ,
		STREAM_COPY = GL_STREAM_COPY, STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ, STATIC_COPY = GL_STATIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW, DYNAMIC_READ = GL_DYNAMIC_READ,
		DYNAMIC_COPY = GL_DYNAMIC_COPY
	};
	/**
	* Handles and simplifies interacting with OpenGL Buffers
	* TODO: Should I be using glm::value_ptr for reading in glm types?
	* it seems to be ok without it, but I'm not sure if that's just coincidence
	*/
	template<BUFFER B>
	class Buffer {
	public:
		Buffer() : mNvals(0) {}
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
		* Create a buffer and allocate an amount of memory but don't write anything
		* must be called with size_t to not interfere with the generic single-item template
		*/
		Buffer(size_t size, USAGE usage) : mNvals(-1) {
			GLuint buf;
			glGenBuffers(1, &buf);
			mHandle = Handle(buf, sDeleter);
			glBindBuffer(static_cast<GLenum>(B), mHandle);
			glBufferData(static_cast<GLenum>(B), size, NULL, static_cast<GLenum>(usage));
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
			glBindBuffer(static_cast<GLenum>(B), mHandle);
			glBufferData(static_cast<GLenum>(B), sizeof(T), &data, static_cast<GLenum>(usage));
		}
		template<class T, size_t N>
		void bufferData(const std::array<T, N> &data, USAGE usage){
			mNvals = N;
			glBindBuffer(static_cast<GLenum>(B), mHandle);
			glBufferData(static_cast<GLenum>(B), N * sizeof(T), &data[0], static_cast<GLenum>(usage));
		}
		template<class T>
		void bufferData(const std::vector<T> &data, USAGE usage){
			mNvals = data.size();
			glBindBuffer(static_cast<GLenum>(B), mHandle);
			glBufferData(static_cast<GLenum>(B), data.size() * sizeof(T), &data[0], static_cast<GLenum>(usage));
		}
		/**
		* Replace some data within the buffer at some offset, note that
		* after this operation we don't know how many "values" are in the buffer
		* and it will be set to -1
		* @param data The data to write
		* @param offset Offset in buffer to write too, in bytes
		*/
		template<class T>
		void bufferSubData(const T &data, size_t offset){
			mNvals = -1;
			glBindBuffer(static_cast<GLenum>(B), mHandle);
			glBufferSubData(static_cast<GLenum>(B), offset, sizeof(T), &data);
		}
		template<class T, size_t N>
		void bufferSubData(const std::array<T, N> &data, size_t offset){
			mNvals = -1;
			glBindBuffer(static_cast<GLenum>(B), mHandle);
			glBufferSubData(static_cast<GLenum>(B), offset, N * sizeof(T), &data[0]);
		}
		template<class T>
		void bufferSubData(const std::vector<T> &data, size_t offset){
			mNvals = -1;
			glBindBuffer(static_cast<GLenum>(B), mHandle);
			glBufferSubData(static_cast<GLenum>(B), offset, data.size() * sizeof(T), &data[0]);
		}
		/**
		* Get the buffer type 
		* @return the buffer type
		*/
		BUFFER type(){
			return B;
		}
		/**
		* Get the number of values, this will be -1 if the buffer
		* was created in such a way that we don't know how many "values" are in it
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
