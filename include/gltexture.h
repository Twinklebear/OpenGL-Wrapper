#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <string>
#include <functional>
#include <SDL_opengl.h>
#include "handle.h"

namespace GL {
	/**
	* Simple handle for interacting with textures
	* TODO: Add information about width and height and such
	*/
	class Texture {
	public:
		///Blank constructor, will set handle to invalid (-1), needed for the 
		///material constructor
		Texture();
		/**
		* Load a texture from a file
		* @param file File to load from
		* @param upload If we should upload the texture onto the GPU now
		*/
		Texture(const std::string &file, bool upload = true);
		/**
		* Load a texture into this object
		* @param file File to load texture from
		*/
		void load(const std::string &file);
		///Load the texture referenced by mFile
		void load();
		///Unload the texture
		void unload();
		///Get file name
		std::string file() const;
		///Pretend to be a GLuint
		operator GLuint();

	private:
		Handle mHandle;
		std::string mFile;
		///The deleter function
		const static std::function<void(GLuint*)> sDeleter;
	};
}

#endif