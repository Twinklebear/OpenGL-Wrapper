#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <string>
#include <functional>
#include <GL/glew.h>
#include <SOIL.h>
#include "handle.h"
#include "glsampler.h"

namespace GL {
	//Enum for texture types
	//TODO: where does GL_TEXTURE_BUFFER fit in?
	enum class TEXTURE { T1D = GL_TEXTURE_1D, T2D = GL_TEXTURE_2D, T3D = GL_TEXTURE_3D,
		T1D_ARRAY = GL_TEXTURE_1D_ARRAY, T2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		RECTANGLE = GL_TEXTURE_RECTANGLE, CUBE_MAP = GL_TEXTURE_CUBE_MAP,
		CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY, T2D_MULTISAMPLE = GL_TEXTURE_2D_MULTISAMPLE, 
		T2D_MULTISAMPLE_ARRAY = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};
	/**
	* Simple handle for interacting with textures
	* TODO: Add information about width and height and such
	* also need to add support for other types of textures via making this a template class
	*/
	template<TEXTURE T>
	class Texture {
	public:
		Texture(){}
		/**
		* Load a texture from a file
		* @param file File to load from
		* @param upload If we should upload the texture onto the GPU now
		*/
		Texture(const std::string &file, bool upload = true, unsigned int flags = 
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT)
				: mFile(file), mFlags(flags)
		{
			if (upload)
				load();
		}
		/**
		* Load a texture into this object
		* @param file File to load texture from
		*/
		void load(const std::string &file, unsigned int flags = 
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT)
		{
			mFile = file;
			mFlags = flags;
			load();
		}
		///Load the texture referenced by mFile with the preset flags
		void load(){
			//This SOIL_load_OGL_texture needs to change based on the type we're loading
			//It looks like SOIL doesn't support 3d images? what about arrays?
			//It looks like lots of specialization needs to be done for each type, perhaps
			//inheritance is a better choice?
			GLuint t = SOIL_load_OGL_texture(mFile.c_str(), SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID, mFlags);
			
			mHandle = Handle(t, sDeleter);
		}
		///Unload the texture
		void unload(){
			mHandle.release();
		}
		///Bind the texture to a texture unit
		void bind(GLenum unit){
			glActiveTexture(unit);
			glBindTexture(static_cast<GLenum>(T), static_cast<GLuint>(mHandle));
		}
		///Bind the texture to a unit along with a sampler
		void bind(GLenum unit, Sampler &sampler){
			glActiveTexture(unit);
			glBindTexture(static_cast<GLenum>(T), static_cast<GLuint>(mHandle));
			glBindSampler(unit - GL_TEXTURE0, sampler);
		}
		///Get file name
		std::string file() const {
			return mFile;
		}
		///Pretend to be a GLuint
		operator GLuint(){
			return static_cast<GLuint>(mHandle);
		}

	private:
		Handle mHandle;
		std::string mFile;
		unsigned int mFlags;
		///The deleter function
		const static std::function<void(GLuint*)> sDeleter;
	};
	template<TEXTURE T>
	const std::function<void(GLuint*)> Texture<T>::sDeleter = 
		[](GLuint *t){ glDeleteTextures(1, t); };
}

#endif
