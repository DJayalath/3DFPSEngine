#include "Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Entity::Entity(const char* model_path, Shader& shader, Camera& camera) : m_model(model_path), m_shader(shader), m_camera(camera) {}

void Entity::Render()
{
	m_shader.setMat4("projection", m_camera.GetProjectionMatrix());
	m_shader.setMat4("view", m_camera.GetViewMatrix());
	m_shader.setMat4("model", m_model_matrix);
	m_model.Draw(m_shader);
}

void Entity::SetPosition(glm::vec3 position)
{
	m_position = position;
	m_model_matrix = glm::translate(glm::mat4(1.0f), m_position) * glm::mat4_cast(m_quat_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
}

void Entity::SetScale(glm::vec3 scale)
{
	m_scale = scale;
	m_model_matrix = glm::translate(glm::mat4(1.0f), m_position) * glm::mat4_cast(m_quat_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
}

void Entity::SetRotation(glm::vec3 euler_rotation)
{
	m_euler_rotation = euler_rotation;
	m_quat_rotation = 
		glm::angleAxis(glm::radians(euler_rotation.z), glm::vec3(0, 0, 1)) * 
		glm::angleAxis(glm::radians(euler_rotation.y), glm::vec3(0, 1, 0)) * 
		glm::angleAxis(glm::radians(euler_rotation.x), glm::vec3(1, 0, 0));
	m_model_matrix = glm::translate(glm::mat4(1.0f), m_position) * glm::mat4_cast(m_quat_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
}

void Entity::Rotate(glm::vec3 euler_rotate)
{
	m_euler_rotation += euler_rotate;
	m_quat_rotation =
		glm::angleAxis(glm::radians(euler_rotate.z), glm::vec3(0, 0, 1)) *
		glm::angleAxis(glm::radians(euler_rotate.y), glm::vec3(0, 1, 0)) *
		glm::angleAxis(glm::radians(euler_rotate.x), glm::vec3(1, 0, 0)) *
		m_quat_rotation;
	m_model_matrix = glm::translate(glm::mat4(1.0f), m_position) * glm::mat4_cast(m_quat_rotation) * glm::scale(glm::mat4(1.0f), m_scale);
}
