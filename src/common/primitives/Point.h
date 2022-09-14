//
// Created by pc on 14.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_POINT_H
#define FUNCTIONVISUALIZER3D_POINT_H


#include "common/WorldObject.h"

class Point : public WorldObject{
public:
    void render() override;
    Point(const glm::vec3 &position);
};


#endif //FUNCTIONVISUALIZER3D_POINT_H
