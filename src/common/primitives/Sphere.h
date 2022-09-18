//
// Created by pc on 27.07.22.
//

#ifndef OPENGLPROJECT_SPHERE_H
#define OPENGLPROJECT_SPHERE_H


#include "common/WorldObject.h"

class Sphere : public WorldObject {
public:
    Sphere(float radius, int sector_count = 40, int stack_count = 40)
            : radius(radius), sector_count(sector_count), stack_count(stack_count) {
        initialize_vertices();
        initialize_normals();
        initialize_indices();
        WorldObject::initialize_buffers();
    }

    float getRadius() const {
        return radius;
    }

protected:
    void initialize_indices();

    void initialize_normals();

    void initialize_vertices();

private:
    float radius;
    GLuint sector_count;
    GLuint stack_count;
};


#endif //OPENGLPROJECT_SPHERE_H
