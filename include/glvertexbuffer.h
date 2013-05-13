#ifndef GLVERTEXBUFFER_H
#define GLVERTEXBUFFER_H

#include <functional>
#include <array>
#include <vector>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include "handle.h"
#include "glfunctions.h"

namespace GL {
	enum BUFFER { ARRAY = GL_ARRAY_BUFFER, ELEMENT = GL_ELEMENT_ARRAY_BUFFER,
		UNIFORM = GL_UNIFORM_BUFFER };
	/**
	* Handles and simplifies interacting with OpenGL Buffers
	*/
	class VertexBuffer {
	public:
		///Blank constructor, literally does nothing, but I need to define it
		VertexBuffer() : mType(BUFFER::ARRAY) {};
		/**
		* Create a new VBO using the data stored in the array passed
		* Currently just doing with GL_STATIC_DRAW since that's all I need but 
		* I'll probably expand to handle all types later
		* @param data The array of data to be passed
		* @param type Type of buffer to create, defaults to array buffer
		*/
		template<class T>
		VertexBuffer(const T &t, BUFFER type = BUFFER::ARRAY)
			: mType(type), mNvals(1)
		{
			GLuint vbo;
			GenBuffers(1, &vbo);
			mHandle = Handle(vbo, sVboDeleter);
			BufferData(t);
		}
		template<class T, size_t N>
		VertexBuffer(const std::array<T, N> &data, BUFFER type = BUFFER::ARRAY)
			: mType(type), mNvals(N)
		{
			GLuint vbo;
			GenBuffers(1, &vbo);
			mHandle = Handle(vbo, sVboDeleter);
			BufferData(data);
		}
		template<class T>
		VertexBuffer(const std::vector<T> &data, BUFFER type = BUFFER::ARRAY)
			: mType(type), mNvals(data.size())
		{
			GLuint vbo;
			GenBuffers(1, &vbo);
			mHandle = Handle(vbo, sVboDeleter);
			BufferData(data);
		}
		/**
		* Create a VBO to handle interaction with an existing object
		* @param vbo The existing vbo to take ownership of
		* @param type The type of buffer being managed
		*/
		VertexBuffer(GLuint vbo, BUFFER type = BUFFER::ARRAY) 
			: mType(type), mHandle(vbo, sVboDeleter)
		{}
		/**
		* Write some data to the buffer, creating a new data store
		* and erasing any previous data
		* @param data The data to write
		*/
		template<class T>
		void BufferData(const T &data){
			BindBuffer(mType, mHandle);
			GL::BufferData(mType, sizeof(T), &data, GL_STATIC_DRAW);
		}
		template<class T, size_t N>
		void BufferData(const std::array<T, N> &data){
			BindBuffer(mType, mHandle);
			GL::BufferData(mType, N * sizeof(T), &data[0], GL_STATIC_DRAW);
		}
		template<class T>
		void BufferData(const std::vector<T> &data){
			BindBuffer(mType, mHandle);
			GL::BufferData(mType, data.size() * sizeof(T), &data[0], GL_STATIC_DRAW);
		}
		/**
		* Update a subset of the buffer's data
		* @param data The data to write
		* @param offset The offset to write too
		*/
		template<class T>
		void BufferSubData(const T &t, size_t offset){
			BindBuffer(mType, mHandle);
			GL::BufferSubData(mType, offset, sizeof(T), &t);
		}
		template<class T, size_t N>
		void BufferSubData(const std::array<T, N> &data, size_t offset){
			BindBuffer(mType, mHandle);
			GL::BufferSubData(mType, offset, N * sizeof(T), &data[0]);
		}
		template<class T>
		void BufferSubData(const std::vector<T> &data, size_t offset){
			BindBuffer(mType, mHandle);
			GL::BufferSubData(mType, offset, data.size() * sizeof(T), &data[0]);
		}
		/**
		* Get the buffer type 
		* @return the buffer type
		*/
		BUFFER Type(){
			return mType;
		}
		/**
		* Get the number of values
		*/
		size_t NumVals(){
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
		BUFFER mType;
		///Number of values in the buffer
		size_t mNvals;
		/**
		* Wrapper around the delete buffers function to make a delete 1 vbo
		* function that can be passed as a handle destructor
		*/
		static const std::function<void(GLuint*)> sVboDeleter;
	};    
}

#endif