#version 330

layout (std140) uniform Globals {
	mat4 p;
	vec4 ambient;
};

in vec3 normal;

out vec4 fragColor;

void main(){
	fragColor = vec4(normal.x, normal.y, normal.z, 1.f);
	fragColor = min(fragColor * ambient, vec4(1.f));
}
