#include "MeshRenderer.h"

void MeshRenderer::Input(Transform transform) {}

void MeshRenderer::Update(Transform transform) {}

void MeshRenderer::Render(Transform transform)
{
	m_shader.use();
	m_shader.setMat4("projection", transform.GetProjectionMatrix());
	m_shader.setMat4("view", transform.GetViewMatrix());
	m_shader.setMat4("model", transform.GetTransformation());
	m_model.Draw(m_shader);
}