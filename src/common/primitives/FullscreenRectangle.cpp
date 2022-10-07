#include "FullscreenRectangle.h"
using namespace glm;
FullscreenRectangle::FullscreenRectangle()
{
	vertices.push_back(vec3(-1.0f, -1.0f, 0));
	vertices.push_back(vec3( 1.0f, -1.0f, 0));
	vertices.push_back(vec3( 1.0f,  1.0f, 0));
	vertices.push_back(vec3(-1.0f,  1.0f, 0));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	initialize_buffers();
}
