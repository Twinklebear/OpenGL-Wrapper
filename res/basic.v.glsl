#version 330

//A very basic vertex shader for testing the ubo stuff
//A block for shared globals between shaders
//ie. the projection/camera matrix, lights etc.
layout (std140) uniform Globals {
	mat4 p;
};

//The model matrix
uniform mat4 m;

in vec4 position;
in vec4 color;
in vec2 texUv;
out vec4 fragColor;
out vec2 uv;

void main(){
	fragColor = color;
	uv = texUv;
	gl_Position = p * m * position;
}
