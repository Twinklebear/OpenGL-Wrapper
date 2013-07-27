#version 330

//This is a simple shader for drawing a cube

layout (std140) uniform Mat {
	mat4 p;
	mat4 m;
};

in vec4 position;

void main(){
	gl_Position = p * m * position;
}
