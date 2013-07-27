#version 330

//Block for the projection and viewing information
layout(std140) uniform PV {
	mat4 p;
	mat4 v;
	//vec4 eyeDir;
};

//The model matrix
uniform mat4 m;
in vec4 vPosition;
in vec3 vNormal;

out vec3 normal;

void main(){
	normal = normalize(m * vec4(vNormal, 0.f)).xyz;
	gl_Position = p * v * m * vPosition;
}
