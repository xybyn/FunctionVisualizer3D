//
// Created by pc on 14.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_LINE_H
#define FUNCTIONVISUALIZER3D_LINE_H


#include "common/WorldObject.h"

class Line : public WorldObject 
{
public:
    void render() override;
    Line(const glm::vec3& p0, const glm::vec3& p1);
    void setP0(const glm::vec3& p0);
    void setP1(const glm::vec3& p1);
    glm::vec3 getP0() const { return vertices[0]; }
    glm::vec3 getP1() const { return vertices[1]; }
};


#endif //FUNCTIONVISUALIZER3D_POINT_H
