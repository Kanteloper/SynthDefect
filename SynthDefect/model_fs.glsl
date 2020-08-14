#version 460

in vec4 vertexColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 lightPos;
out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 viewPos;
//uniform vec3 lightPos;

void main()
{    
	// ambient lightning
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse lightning
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);		// note that max function is for always positive value
	vec3 diffuse = diff * lightColor;

	// specular lightning
	float specularStrength = 0.7;
	vec3 viewDir = normalize(viewPos - lightPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * vec3(vertexColor);
	FragColor = vec4(result, 1.0);
}