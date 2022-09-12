//
// Created by pc on 27.07.22.
//

#include "Sphere.h"
#include "common/shaders/DefaultShader.h"

void Sphere::initialize_indices() {
    for (uint i = 0; i < stack_count; ++i) {
        uint k1 = i * (sector_count + 1);
        uint k2 = k1 + sector_count + 1;

        for (uint j = 0; j < sector_count; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i == stack_count - 1) {
                continue;
            }
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }
}

void Sphere::initialize_normals() {

}

void Sphere::initialize_vertices() {

    float sector_step = 2.0f * glm::pi<float>() / sector_count;
    float stack_step = glm::pi<float>() / stack_count;

    float phi = 0.0f;

    float xy;
    float z;
    float x, y;
    float nx, ny, nz;
    float stack_angle, sector_angle;
    float lengthInv = 1.0f / radius;
    for (int i = 0; i <= stack_count; i++) {
        stack_angle = (glm::pi<float>() * 0.5) - (i * stack_step); // starting from pi/2 to -pi/2
        xy = radius * cos(stack_angle);                            // r * cos(u)
        z = radius * sin(stack_angle);                             // r * sin(u)

        for (int j = 0; j <= sector_count; ++j) {
            sector_angle = j * sector_step; // starting from 0 to 2pi

            x = xy * cos(sector_angle); // r * cos(u) * cos(v)
            y = xy * sin(sector_angle); // r * cos(u) * sin(v)
            vertices.push_back(glm::vec3(x, y, z));
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(glm::vec3(nx, ny, nz));

            tex_coords.push_back(glm::vec2((float)j / sector_count, (float)i / stack_count));
        }
    }
}
