#ifndef OPENGLPROJECT_BOUNDBOX_H
#define OPENGLPROJECT_BOUNDBOX_H
#include <glm/common.hpp>
#include <vector>
struct BoundBox
{
    glm::vec3 left_bottom_back;
    glm::vec3 right_upper_front;
    BoundBox(const glm::vec3 & left_bottom_back, const glm::vec3 &right_upper_front);
    bool contains(const glm::vec3 &p, float eps = 0.0f) const;
    bool intersects(const BoundBox &other_bb) const;
    float getSizeX() const;
    float getSizeY() const;
    float getSizeZ() const;

    glm::vec3 getLeftBottomBack() const;
    glm::vec3 getRightBottomBack() const;
    glm::vec3 getRightBottomFront() const;
    glm::vec3 getLeftBottomFront() const;

    glm::vec3 getLeftUpperBack() const;
    glm::vec3 getRightUpperBack() const;
    glm::vec3 getRightUpperFront() const;
    glm::vec3 getLeftUpperFront() const;
};
#endif // OPENGLPROJECT_BOUNDBOX_H
