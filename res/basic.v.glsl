#version 330

//A very basic vertex shader for testing the ubo stuff
layout (std140) uniform Mat {
	mat4 p;
	mat4 m;
};

in vec4 position;

void main(){
	gl_Position = p * m * position;
}
