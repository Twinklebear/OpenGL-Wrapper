#version 330

//A block for shared globals between shaders
//ie. the projection/camera matrix, lights etc.
layout (std140) uniform Globals {
	mat4 p;
	vec4 ambient;
};

//The model matrix
uniform mat4 m;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vUv;

out vec2 uv;
out vec3 normal;

void main(){
	uv = vUv;
	normal = normalize(m * vNormal).xyz;
	gl_Position = p * m * vPosition;
}
