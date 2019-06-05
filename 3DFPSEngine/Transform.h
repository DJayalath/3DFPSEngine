#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Transform
{
private:
	Transform* m_parent = nullptr;
	glm::mat4 m_parent_matrix;

	glm::vec3 m_pos;
	glm::quat m_rot;
	glm::vec3 m_scale;

	glm::vec3 m_old_pos;
	glm::quat m_old_rot;
	glm::vec3 m_old_scale;

	glm::mat4 GetParentMatrix()
	{
		if (m_parent != nullptr && m_parent->HasChanged())
			m_parent_matrix = m_parent->GetTransformation();

		return m_parent_matrix;
	}

	static glm::mat4 m_projection_matrix;
	static glm::mat4 m_view_matrix;

public:

	Transform()
	{
		
	}

	void Update()
	{
		m_old_pos = m_pos;
		m_old_rot = m_rot;
		m_old_scale = m_scale;
	}

	void Rotate(glm::vec3 axis, float angle)
	{
		m_rot = glm::normalize(glm::angleAxis(angle, axis) * m_rot);
	}

	void LookAt(glm::vec3 point, glm::vec3 up)
	{
		m_rot = glm::lookAt(m_pos, point, up);
	}

	bool HasChanged()
	{
		if (m_parent != nullptr && m_parent->HasChanged())
			return true;

		if (m_pos != m_old_pos)
			return true;

		if (m_rot != m_old_rot)
			return true;

		if (m_scale != m_old_scale)
			return true;

		return false;
	}

	glm::mat4 GetTransformation()
	{
		// Is glm::mat4(1.f) the identity matrix???
		glm::mat4 translation_matrix = glm::translate(glm::mat4(1.f), m_pos);
		glm::mat4 rotation_matrix = glm::toMat4(m_rot);
		glm::mat4 scale_matrix = glm::scale(glm::mat4(1.f), m_scale);

		return m_parent_matrix * (translation_matrix * rotation_matrix * scale_matrix); // CORRECT ORDER???
	}

	void SetParent(Transform parent)
	{
		m_parent = &parent; // Careful Ned, Careful now...
	}

	glm::vec3 GetTransformedPos()
	{
		return glm::mat3(GetParentMatrix()) * m_pos; // Assuming transform = mult
	}

	glm::quat GetTransformedRot()
	{
		glm::quat parent_rotation;

		if (m_parent != nullptr)
			parent_rotation = m_parent->GetTransformedRot();

		return parent_rotation * m_rot;
	}

	glm::vec3 GetPos()
	{
		return m_pos;
	}

	void SetPos(glm::vec3 pos)
	{
		m_pos = pos;
	}

	glm::quat GetRot()
	{
		return m_rot;
	}

	void SetRot(glm::quat rot)
	{
		m_rot = rot;
	}

	glm::vec3 GetScale()
	{
		return m_scale;
	}

	void SetScale(glm::vec3 scale)
	{
		m_scale = scale;
	}

	static void SetProjectionMatrix(glm::mat4 mat)
	{
		m_projection_matrix = mat;
	}

	static void SetViewMatrix(glm::mat4 mat)
	{
		m_view_matrix = mat;
	}

	static glm::mat4 GetProjectionMatrix()
	{
		return m_projection_matrix;
	}

	static glm::mat4 GetViewMatrix()
	{
		return m_view_matrix;
	}
};

