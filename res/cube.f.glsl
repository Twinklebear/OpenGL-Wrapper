#version 330

struct DirectionalLight {
	vec4 ambient;
	vec4 color;
	vec4 direction;
	vec4 halfVector;
	float strength;
};
struct PointLight {
	vec4 ambient;
	vec4 color;
	vec4 position;
	float strength;
	float constAtten;
	float linearAtten;
	float quadraticAtten;
};

//Block for the projection and viewing information
layout(std140) uniform PV {
	mat4 p;
	mat4 v;
	vec3 eyePos;
};

//Block for lighting information
layout(std140) uniform Lighting {
	PointLight pointLight;
	DirectionalLight dirLight;
};

in vec4 position;
in vec3 normal;

out vec4 fragColor;

void main(){
	//Is this right? I'm familiar with the light direction and half vector pointing away
	//from the object, this looks like they're doing it with it pointing to the object
	vec3 lightDir = vec3(v * pointLight.position) - vec3(position);
	float lightDistance = length(lightDir);
	//Normalize the light direction
	lightDir = lightDir / lightDistance;

	float attenuation = 1.f / (pointLight.constAtten + pointLight.linearAtten * lightDistance
		+ pointLight.quadraticAtten * pow(lightDistance, 2.f));

	//Eyedir is probably ok to do per-vertex
	vec3 eyeDir = eyePos - vec3(position);
	vec3 halfVector = normalize(lightDir + eyeDir);

	float diffuse = max(0.f, dot(normal, lightDir));
	float specular = max(0.f, dot(normal, halfVector));

	if (diffuse == 0.f)
		specular = 0.f;
	else
		//Some arbitrary constant shininess
		specular = pow(specular, 10.f) * pointLight.strength;

	vec4 scattered = pointLight.ambient + pointLight.color * diffuse * attenuation;
	vec4 reflected = pointLight.color * specular * attenuation;

	fragColor = min(vec4(normal, 1.f) * scattered + reflected, vec4(1.f));
}
