//
// Created by pc on 02.08.22.
//

#ifndef OPENGLPROJECT_PLANE_H
#define OPENGLPROJECT_PLANE_H


#include "common/WorldObject.h"

class Plane : public WorldObject{
public:
    Plane(float width, float height);
private:
    float width, height;
};


#endif //OPENGLPROJECT_PLANE_H
