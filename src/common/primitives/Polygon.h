//
// Created by pc on 14.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_POLYGON_H
#define FUNCTIONVISUALIZER3D_POLYGON_H


#include "common/WorldObject.h"
#include <vector>
class Polygon : public WorldObject {
public:
    void render() override;
    Polygon(const std::vector<glm::vec3>& points);
};


#endif //FUNCTIONVISUALIZER3D_POINT_H
