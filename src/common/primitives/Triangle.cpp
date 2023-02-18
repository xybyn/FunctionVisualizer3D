#include "Triangle.h"

void Triangle::render()
{
	WorldObject::render();
}

Triangle::Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
{
	vertices.push_back(p0);
	vertices.push_back(p1);
	vertices.push_back(p2);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	glm::vec3 v0 = p1 - p0;
	glm::vec3 v1 = p2 - p1;
	glm::vec3 normal = glm::normalize(glm::cross(v0, v1));
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);

	initialize_buffers();
}
