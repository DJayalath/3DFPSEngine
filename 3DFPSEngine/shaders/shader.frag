#version 330 core
in vec3 f_normal;
in vec3 f_pos;
out vec4 FragColor;
 
uniform vec3 lightPos; 
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	// Ambient Lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

	// Diffuse Lighting (based on angle to normal)
	vec3 norm = normalize(f_normal);
	vec3 lightDir = normalize(lightPos - f_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular Lighting (based on viewing angle to reflection angle)
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - f_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	// 32 is shine amount, increase or decrease by halving or doubling
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor; 

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}