//
// Created by pc on 14.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_TRIANGLE_H
#define FUNCTIONVISUALIZER3D_TRIANGLE_H


#include "common/WorldObject.h"
#include <vector>
class Triangle : public WorldObject {
public:
    void render() override;
    Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
};


#endif //FUNCTIONVISUALIZER3D_POINT_H
