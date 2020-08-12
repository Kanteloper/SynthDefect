#version 460

in vec4 vertexColor; 
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{    
	float ambientStrength = 1.0;
	vec3 ambient = ambientStrength * lightColor;
	vec4 result = vec4(ambient, 1.0) * vertexColor;
	FragColor = result;
}