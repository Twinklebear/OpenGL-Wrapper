#version 330

layout (std140) uniform VP {
	mat4 v;
	mat4 p;
};

layout (std140) uniform Lighting {
	vec4 lightPos;
};

uniform mat4 m;
//Color inputs
uniform vec3 kA, kD, kS;
uniform float nS;
//Texture inputs, 0: ambient, 1: diffuse, 2: specular
//uniform sampler2D samplers[3];
uniform sampler2D tex;
	
in vec4 vPos;
in vec3 vNorm;
in vec3 texCoord;
out vec4 outColor;

void main(){
	//Get normal in world coords
	vec3 normal = vec3(normalize(m * vec4(vNorm, 0.0f)));
	//calculate our location in world coords
	vec4 position = m * vPos;

	//Vector from pos to light
	vec3 l = vec3(lightPos - position);
	float diffuse = clamp(dot(normal, l), 0, 1);

	//I could probably not do this in the shader, since the halfway
	//vector will only change when the light or view moves
	//does this really give me the viewing direction?
	vec3 viewDir = vec3(v * vec4(normal, 0.0f));
	vec3 halfway = normalize(l + viewDir);
	float nDoth = dot(normal, halfway);
	float spec = pow(clamp(nDoth, 0, 1), nS);

	//Compute final color
	outColor = spec * texture(tex, texCoord.xy) 
		+ diffuse * texture(tex, texCoord.xy)
		+ 0.2f * texture(tex, texCoord.xy);
}
