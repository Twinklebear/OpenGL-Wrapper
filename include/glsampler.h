#ifndef GLSAMPLER_H
#define GLSAMPLER_H

#include "handle.h"

namespace GL {
	/**
	* A wrapper for sampler objects
	*/
	class Sampler {
	public:
		///Create a new sampler
		Sampler();
		///Set a parameter for the sampler
		void parameteri(GLenum pname, int param);
		void parameterf(GLenum pname, float param);
		operator GLuint();

	private:
		Handle mHandle;
		const static std::function<void(GLuint*)> sDeleter;
	};
}

#endif
