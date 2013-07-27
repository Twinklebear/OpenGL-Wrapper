#version 330

uniform sampler2D tex2D;

in vec3 normal;
in vec2 uv;

out vec4 color;

void main(){
	color = texture(tex2D, uv) + 0.4f * vec4(normal.x, normal.y, normal.z, 1.f);
}
