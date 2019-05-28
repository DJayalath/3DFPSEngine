#pragma once

#include <glad/glad.h> 
#include "Model.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Entity
{
public:
	Entity(const char* model_path, Shader& shader, Camera& camera);

	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetScale() { return m_scale; }

	void SetPosition(glm::vec3 position);
	void SetScale(glm::vec3 scale);

	void SetRotation(glm::vec3 euler_rotation);
	void Rotate(glm::vec3 euler_rotate);
	glm::vec3 GetRotation() { return m_euler_rotation; }

	void Render();

private:
	Model m_model;
	Shader& m_shader;
	Camera& m_camera;
	glm::mat4 m_model_matrix;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_euler_rotation;
	glm::quat m_quat_rotation;
};

