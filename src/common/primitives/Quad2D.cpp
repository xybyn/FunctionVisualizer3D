//
// Created by pc on 16.09.22.
//

#include "Quad2D.h"
using namespace  glm;
Quad2D::Quad2D()
{
    vertices.push_back(vec3(-1, -1, 0));
    vertices.push_back(vec3( 1, -1, 0));
    vertices.push_back(vec3( 1,  1, 0));
    vertices.push_back(vec3(-1,  1, 0));

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);

    tex_coords.push_back(vec2(0, 0));
    tex_coords.push_back(vec2(1, 0));
    tex_coords.push_back(vec2(1, 1));
    tex_coords.push_back(vec2(0, 1));

    initialize_buffers();
}

