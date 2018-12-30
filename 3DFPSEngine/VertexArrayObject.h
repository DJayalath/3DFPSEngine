#pragma once

#include <glad/glad.h>

class VertexArrayObject
{
public:
	VertexArrayObject(GLenum mode, unsigned int indices, float* vertices);
	~VertexArrayObject();

	void Bind();
	void Draw();

private:
	unsigned int VAO, VBO;
	unsigned int m_indices;
	GLenum m_mode;
};

VertexArrayObject::VertexArrayObject(GLenum mode, unsigned int indices, float* vertices) : m_mode(mode), m_indices(indices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 1. Bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. Copy vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 4. Set the vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//// Safely unbind the VBO and VAO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject()
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
}

void VertexArrayObject::Bind()
{
	glBindVertexArray(VAO);
}

void VertexArrayObject::Draw()
{
	// Insert transformations here
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// glBindVertexArray(0)
}
