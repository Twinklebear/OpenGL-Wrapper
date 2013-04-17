//#version 420
//laptop only supports 330
//Actually is now able to support 400
#version 330

uniform mat4 m;
uniform mat4 v;
uniform vec4 lightPos;
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
	float intensity = clamp(dot(normal, l), 0, 1);

	//I could probably not do this in the shader, since the halfway
	//vector will only change when the light or view moves
	//does this really give me the viewing direction?
	vec3 viewDir = vec3(v * vec4(normal, 0.0f));
	vec3 halfway = normalize(l + viewDir);
	float nDoth = dot(normal, halfway);
	float spec = pow(clamp(nDoth, 0, 1), 50);

	vec4 color = texture(tex, texCoord.xy);
	//Add diffuse and ambient
	outColor = spec * color + color * intensity + color * 0.2f;
}
