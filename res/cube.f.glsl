#version 330

in vec3 normal;

out vec4 outColor;

void main(){
	outColor = vec4(normal.x, normal.y, normal.z, 1.f);
}
