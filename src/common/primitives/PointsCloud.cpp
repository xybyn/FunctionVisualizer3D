#include "Triangle.h"
#include "PointsCloud.h"

using namespace glm;
void PointsCloud::render()
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
	glDrawArrays(GL_POINTS, 0, vertices.size());
}
PointsCloud::PointsCloud(const std::vector<glm::vec4>& points)
{
	for (int i = 0; i < points.size(); i++)
	{
		vertices.push_back(vec3(points[i]));
	}
	initialize_buffers();
}
PointsCloud::PointsCloud(const std::vector<glm::vec3>& points)
{
	for (int i = 0; i < points.size(); i++)
	{
		vertices.push_back(points[i]);
	}
	initialize_buffers();
}
