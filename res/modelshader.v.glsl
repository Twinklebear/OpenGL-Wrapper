#version 330

//Shared viewing and projection matrices since all
//object will have the same v & p
layout (std140) uniform VP {
	mat4 v;
	mat4 p;
};

//Model transform matrix
uniform mat4 m;

in vec3 position;
in vec3 normal;
in vec3 texIn;
out vec3 vNorm;
out vec4 vPos;
out vec3 texCoord;

void main(){
	//Pass fragment shader vars
	vNorm = normal;
	vPos = vec4(position, 1.0f);
	texCoord = texIn;
	//Set position
	gl_Position = p * v * m * vec4(position, 1.0f);
}
