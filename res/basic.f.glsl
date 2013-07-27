#version 330

layout (std140) uniform Globals {
	mat4 p;
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
