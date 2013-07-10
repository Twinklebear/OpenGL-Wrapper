#ifndef GLSHADER_H
#define GLSHADER_H

#include <string>
#include <GL/glew.h>
#include "handle.h"
#include "util.h"

namespace GL {
	//The various shader types
    enum class SHADER { VERTEX = GL_VERTEX_SHADER, TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TESS_EVAL = GL_TESS_EVALUATION_SHADER, GEOMETRY = GL_GEOMETRY_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER, COMPUTE = GL_COMPUTE_SHADER
	};
    /**
    * Handles and simplifies interacting with OpenGL Shader objects
    */
	template<SHADER S>
    class Shader {
    public:
		//Blank constructor, don't do anything
		Shader() {}
        /**
        * Create a new shader of the desired type using the contents
        * of file as the shader source code
        * @param file The shader source filename
        */
		Shader(const std::string &file){
			create(file);
		}
        /**
        * Debug a shader, check if it compiled successfully and get back
        * the shader info log
        * @return The shader info log
        */
		std::string getLog(){
			if (!status()){
				//Get the log length and then get the log
				GLint logLength;
				glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &logLength);
				std::vector<char> log(logLength);
				glGetShaderInfoLog(mHandle, logLength, NULL, &log[0]);

				//Setup our error string
				std::string errorMsg = "";
				switch (S){
				case SHADER::VERTEX:
					errorMsg = "Vertex shader error log:\n";
					break;
				case SHADER::TESS_CONTROL:
					errorMsg = "Tess_control shader error log:\n";
					break;
				case SHADER::TESS_EVAL:
					errorMsg = "Tess_eval shader error log:\n";
					break;
				case SHADER::GEOMETRY:
					errorMsg = "Geometry shader error log:\n";
					break;
				case SHADER::FRAGMENT:
					errorMsg = "Fragment shader error log:\n";
					break;
				case SHADER::COMPUTE:
					errorMsg = "Compute shader error log:\n";
					break;
				default:
					break;
				}
				//Construct and return log message
				errorMsg += std::string(log.begin(), log.end());
				return errorMsg;
			}
			return "Success";
		}
        /**
        * Check the status of compiling the shader, True if ok
        * @return True if compiled successfully
        */
		bool status(){
			GLint status;
			glGetShaderiv(mHandle, GL_COMPILE_STATUS, &status);
			return (status == GL_TRUE);
		}
        /**
        * Implicitly convert to a GLuint if trying to use
        * the shader as such
        */
		operator GLuint(){
			return mHandle;
		}

    private:
        /**
        * Create a shader program of some type using the contents
        * of some file
        * @param file The file to read contents from
        * @param type The type of shader to create
        */
		void create(const std::string &file){
			mHandle = Handle(glCreateShader(static_cast<GLenum>(S)), sDeleter);
			std::string src = Util::readFile(file);
			const char *srcPtr = src.c_str();
			glShaderSource(mHandle, 1, &srcPtr, NULL);
			glCompileShader(mHandle);
		}
    
    private:
        Handle mHandle;
		//The shader deleter function
		const static std::function<void(GLuint*)> sDeleter;
    };
	template<SHADER S>
	const std::function<void(GLuint*)> Shader<S>::sDeleter = [](GLuint *s){ glDeleteShader(*s); };

	//Some convenient typedefs
	typedef Shader<SHADER::VERTEX> VertexShader;
	typedef Shader<SHADER::TESS_CONTROL> TessControlShader;
	typedef Shader<SHADER::TESS_EVAL> TessEvalShader;
	typedef Shader<SHADER::GEOMETRY> GeometryShader;
	typedef Shader<SHADER::FRAGMENT> FragmentShader;
	typedef Shader<SHADER::COMPUTE> ComputeShader;
}

#endif
