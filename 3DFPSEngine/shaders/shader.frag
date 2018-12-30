#version 330 core
in vec3 f_normal;
in vec3 f_pos;
out vec4 FragColor;
 
uniform vec3 objectColor;
uniform vec3 viewPos;

// Material defined property strengths
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

// Light intensities
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light; 

void main()
{
	// Ambient Lighting
    vec3 ambient = light.ambient * material.ambient;

	// Diffuse Lighting (based on angle to normal)
	vec3 norm = normalize(f_normal);
	vec3 lightDir = normalize(light.position - f_pos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular Lighting (based on viewing angle to reflection angle)
	vec3 viewDir = normalize(viewPos - f_pos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}