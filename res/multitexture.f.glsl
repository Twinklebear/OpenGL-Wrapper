#version 330

uniform sampler2D texA;
uniform sampler2D texB;

//A basic fragment shader, just sets to white
in vec4 fragColor;
in vec2 uv;
out vec4 color;

void main(){
	color = texture(texA, uv) * texture(texB, uv) + 0.1f * fragColor;
}
