#version 330

//A block for shared globals between shaders
//ie. the projection/camera matrix, lights etc.
layout (std140) uniform Globals {
	mat4 p;
};

//The model matrix
uniform mat4 m;
in vec4 position;

void main(){
	gl_Position = p * m * position;
}
