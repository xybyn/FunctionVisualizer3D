//
// Created by pc on 02.08.22.
//

#include "Plane.h"
#include <glm/vec3.hpp>

using namespace glm;

Plane::Plane(float width, float height)
        : width(width), height(height) {
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    vertices.push_back(vec3(-hw, 0, -hh));
    vertices.push_back(vec3( hw, 0, -hh));
    vertices.push_back(vec3( hw, 0, hh));
    vertices.push_back(vec3(-hw, 0, hh));
    normals.push_back(vec3(0, 1, 0));
    normals.push_back(vec3(0, 1, 0));
    normals.push_back(vec3(0, 1, 0));
    normals.push_back(vec3(0, 1, 0));
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    initialize_buffers();
}
