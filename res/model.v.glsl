#version 330

//Block for the projection and viewing information
layout(std140) uniform PV {
	mat4 p;
	mat4 v;
	vec3 eyePos;
};

//The model matrix
uniform mat4 modelMat;
uniform mat3 normalMat;

in vec4 vPosition;
in vec3 vNormal;

out vec3 normal;
out vec4 position;

void main(){
	normal = normalize(normalMat * vNormal);
	position = v * modelMat * vPosition;
	gl_Position = p * v * modelMat * vPosition;
}
