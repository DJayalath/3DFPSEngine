#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Default camera attributes
static constexpr float YAW = -90.0f;
static constexpr float PITCH = 0.0f;
static constexpr float SPEED = 2.5f;
static constexpr float SENSITIVITY = 0.1f;
static constexpr float ZOOM = 70.0f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, float dt);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch = true);
	void ProcessMouseScroll(float yoffset);

	float GetZoom()
	{
		return m_zoom;
	}
	glm::vec3 GetPosition()
	{
		return m_position;
	}

private:

	void UpdateCameraVectors();

	// Camera Attributes
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_world_up;
	// Euler Angles
	float m_yaw;
	float m_pitch;
	// Camera options
	float m_speed;
	float m_sensitivity;
	float m_zoom;
};

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_speed(SPEED), m_sensitivity(SENSITIVITY), m_zoom(ZOOM)
{
	m_position = position;
	m_world_up = up;
	m_yaw = yaw;
	m_pitch = pitch;
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float dt)
{
	float velocity = m_speed * dt;
	if (direction == FORWARD)
		m_position += m_front * velocity;
	if (direction == BACKWARD)
		m_position -= m_front * velocity;
	if (direction == LEFT)
		m_position -= m_right * velocity;
	if (direction == RIGHT)
		m_position += m_right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrain_pitch)
{
	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	m_yaw += xoffset;
	m_pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrain_pitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	UpdateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	// Calculate new front vector
	m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front.y = sin(glm::radians(m_pitch));
	m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(m_front);

	// Use cross product to calculate perpendicular vectors
	m_right = glm::normalize(glm::cross(m_front, m_world_up));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}
