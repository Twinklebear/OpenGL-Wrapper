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

uniform sampler2D tex2D;

in vec3 normal;
in vec2 uv;

out vec4 fragColor;

void main(){
	fragColor = 0.5f * texture(tex2D, uv) + 0.5f * vec4(normal.x, normal.y, normal.z, 1.f);
	fragColor = min(fragColor * ambient, vec4(1.f));
}
