#version 330

uniform sampler2D tex2D;

in vec4 fragColor;
in vec2 uv;
out vec4 color;

void main(){
	color = texture(tex2D, uv) + 0.1f * fragColor;
}
