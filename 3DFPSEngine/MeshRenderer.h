#pragma once
#include "GameComponent.h"
#include "Model.h"

class MeshRenderer : public GameComponent
{
private:
	Model m_model;
	Shader& m_shader;

public:
	MeshRenderer(const char* model_path, Shader& shader) : m_model(model_path), m_shader(shader) {}

	void Input(Transform transform);
	void Update(Transform transform);
	void Render(Transform transform);
};

