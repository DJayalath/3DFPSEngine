#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
  
uniform mat4 MVPMatrix;
uniform mat4 ModelMatrix;
uniform mat4 InverseModelMatrix;

out vec3 f_normal;
out vec3 f_pos;

void main()
{
    gl_Position = MVPMatrix * vec4(position, 1.0f);
	f_pos = vec3(ModelMatrix * vec4(position, 1.0));
	f_normal = mat3(transpose(InverseModelMatrix)) * normal;
} 