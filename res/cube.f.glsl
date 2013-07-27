#version 330

//Block for the projection and viewing information
layout(std140) uniform PV {
	mat4 p;
	mat4 v;
	// vec4 eyeDir;
};

//Block for lighting information
layout(std140) uniform Lighting {
	vec4 ambient;
};

in vec3 normal;

out vec4 fragColor;

void main(){
	fragColor = vec4(normal.x, normal.y, normal.z, 1.f);
	fragColor = min(fragColor * ambient, vec4(1.f));
}
