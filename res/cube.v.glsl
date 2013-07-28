#version 330

//Block for the projection and viewing information
layout(std140) uniform PV {
	mat4 p;
	mat4 v;
	//vec4 eyeDir;
};

//The model matrix
uniform mat4 modelMat;
uniform mat3 normalMat;

in vec4 vPosition;
in vec3 vNormal;

out vec3 normal;

void main(){
	normal = normalize(normalMat * vNormal);
	gl_Position = p * v * modelMat * vPosition;
}
