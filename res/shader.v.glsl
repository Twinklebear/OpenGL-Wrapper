//#version 420
//laptop only supports 330
//Actually is now able to support 400
#version 330

//Model transform matrix
uniform mat4 m;
//View matrix
uniform mat4 v;
//Projection matrix
uniform mat4 p;

in vec3 position;
in vec3 normal;
in vec3 texIn;
out vec3 vNorm;
out vec4 vPos;
out vec3 texCoord;

void main(){
	//Pass fragment shader vars
	vNorm = normal;
	vPos = vec4(position, 1.0f);
	texCoord = texIn;
	//Set position
	gl_Position = p * v * m * vec4(position, 1.0f);
}
