#version 330

//Block for the projection and viewing information
layout(std140) uniform PV {
	mat4 p;
	mat4 v;
	// vec4 eyeDir;
};

//The model matrix
uniform mat4 modelMat;
uniform mat4 normalMat;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vUv;

out vec2 uv;
out vec3 normal;

void main(){
	uv = vUv;
	normal = normalize((normalMat * vNormal).xyz);
	gl_Position = p * v * modelMat * vPosition;
}
