//
// Created by pc on 14.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_POINTSCLOUD_H
#define FUNCTIONVISUALIZER3D_POINTSCLOUD_H


#include "common/WorldObject.h"
#include <vector>
class PointsCloud : public WorldObject {
public:
    void render() override;
    PointsCloud(const std::vector<glm::vec3>& points);
    PointsCloud(const std::vector<glm::vec4>& points);
};


#endif //FUNCTIONVISUALIZER3D_POINT_H
