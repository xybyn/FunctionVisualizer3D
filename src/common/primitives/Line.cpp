#include "Line.h"

void Line::render()
{
	if (shader)
	{
		shader->use();
		if (parent)
			shader->setMat4("transform", parent->getTransform() * transform_matrix);
		else
			shader->setMat4("transform", transform_matrix);
	}
	if (texture)
		texture->bind();
	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

Line::Line(const glm::vec3& p0, const glm::vec3& p1)
{
	vertices.push_back(p0);
	vertices.push_back(p1);
	indices.push_back(0);
	indices.push_back(1);
	initialize_buffers();
}