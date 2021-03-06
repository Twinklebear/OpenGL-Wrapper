#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "handle.h"
#include "glbuffer.h"
#include "glshader.h"

namespace GL {
    /**
    * Handles and simplifies interaction with OpenGL
    * shader program objects
    */
    class Program {
    public:
		///Default constructor, will be an invalid handle
		Program();
        /**
        * Create a shader program using vertex and fragment shader files
        * @param vertex The vertex shader file
        * @param frag The fragment shader file
        */
        Program(std::string vertex, std::string frag);
        /**
        * Create a shader program from existing vertex and fragment shaders
        * The function assumes that the shaders being passed have 
        * been compiled successfully. The shaders will be detached before
		* returning from link
        * @param vertex The vertex shader
        * @param frag The fragment shader
        */
        Program(VertexShader &vert, FragmentShader &frag);
		/**
		* Link the program from two shaders and detach the shaders after linking
		* @param vert Vertex shader
		* @param frag Fragment shader
		*/
		void link(VertexShader &vert, FragmentShader &frag);
        /**
        * Debug a program, check if it linked successfully and return the 
        * program log
        * @return The shader info log
        */
        std::string getLog();
        /**
        * Check the status of compiling the shader, True if ok
        * @return True if compiled successfully
        */
        bool status();
		///Use the program
		void use();
        /**
        * Get the location of an attribute with some name
        * @param name The name of the attribute to get the location of
        */
        GLint getAttribute(const std::string &name);
		/**
		* Get the location of a uniform with some name
		*/
		GLint getUniform(const std::string &name);
		/**
		* Get the location of a uniform block  with some name
		* @param name Name of the uniform block to get location of
		*/
		GLint getUniformBlockIndex(const std::string &name);
		/**
		* Bind a buffer to an uniform block in the program. For now this will
		* only work with UniformBlocks too I think since that's the only pre-4.3 block we
		* can use from this side.
		* glGetProgramResourceIndex is needed for shader storage blocks?
		* @param name Name of the block to bind too
		* @param buf The uniform buffer to bind
		*/
		void bindUniformBlock(const std::string &name, const UniformBuffer &buf);
        //TODO: Come up with a better idea for making all these methods
        //or can i?
		void uniform1i(const std::string &attrib, int i);
		///Pass a float as a uniform to some attrib
		void uniform1f(const std::string &attrib, float f);
		/**
		* Pass a glm::vec3 as a uniform
		* @param attrib Name of the attribute to set
		* @param vec Vector to be passed
		*/
		void uniform3fv(const std::string &attrib, const glm::vec3 &vec);
        /**
        * Pass a glm::vec4 as a uniform 
        * @param attrib the name of the attrib to set
        * @param vec The vector to be passed
        */
        void uniform4fv(const std::string &attrib, const glm::vec4 &vec);
        /**
        * Pass a glm::mat as a uniform attribute
        * @param attrib The name of the uniform to set
        * @param matrix The matrix to be passed
        */
        void uniformMat4x4(const std::string &attrib, const glm::mat4 &matrix);
		void uniformMat3x3(const std::string &attrib, const glm::mat3 &matrix);
        /**
        * Implicitly convert to a GLuint if trying to use
        * the program as such
        */
        operator GLuint();

    private:
        Handle mHandle;
		//Deleter function for the handle
		const static std::function<void(GLuint*)> sProgDelete;
    };
}

#endif