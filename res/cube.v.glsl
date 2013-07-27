#version 330

//A block for shared globals between shaders
//ie. the projection/camera matrix, lights etc.
layout (std140) uniform Globals {
	mat4 p;
};

//The model matrix
uniform mat4 m;
in vec4 vPosition;
in vec3 vNormal;

out vec3 normal;

void main(){
	normal = normalize(m * vec4(vNormal, 0.f)).xyz;
	gl_Position = p * m * vPosition;
}
