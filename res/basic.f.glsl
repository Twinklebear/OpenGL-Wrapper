#version 330

//Defintion for a directional light
struct DirectionalLight {
	vec4 ambient;
	vec4 color;
	vec4 direction;
	vec4 halfVector;
};

//Block for the projection and viewing information
layout(std140) uniform PV {
	mat4 p;
	mat4 v;
	// vec4 eyeDir;
};

//Block for lighting information
layout(std140) uniform Lighting {
	DirectionalLight light;
};

uniform sampler2D tex2D;

in vec3 normal;
in vec2 uv;

out vec4 fragColor;

void main(){
	float diffuse = max(0.f, dot(normal, light.direction.xyz));
	float specular = max(0.f, dot(normal, light.halfVector.xyz));
	//Surfaces with no diffuse should also have no spec b/c they're facing the wrong way
	if (diffuse == 0.f)
		specular = 0.f;
	else {
		//Hardcode shininess to some value for now
		specular = pow(specular, 30.f);
	}
	vec4 scattered = light.ambient + light.color * diffuse;
	//add 5 strength, hardcoded for now but this should be added to the light as a property
	vec4 reflected = light.color * specular * 5.f;

	fragColor = min(texture(tex2D, uv) * scattered + reflected, vec4(1.f));
}
