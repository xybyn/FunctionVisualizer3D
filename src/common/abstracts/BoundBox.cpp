#include "BoundBox.h"
#include <iostream>
#include "common/Utils.h"
using namespace glm;


bool BoundBox::contains(const glm::vec3 &p, float eps) const {
    return
            in_range(p.x, left_bottom_back.x, right_upper_front.x, eps) &&
            in_range(p.y, left_bottom_back.y, right_upper_front.y, eps) &&
            in_range(p.z, left_bottom_back.z, right_upper_front.z, eps);
}

bool BoundBox::intersects(const BoundBox &other_bb) const {
    bool not_inside_x =
            other_bb.left_bottom_back.x > right_upper_front.x || other_bb.right_upper_front.x < left_bottom_back.x;

    bool not_inside_y =
            other_bb.left_bottom_back.y > right_upper_front.y || other_bb.right_upper_front.y < left_bottom_back.y;

    bool not_inside_z =
            other_bb.left_bottom_back.z > right_upper_front.z || other_bb.right_upper_front.z < left_bottom_back.z;
    return !not_inside_x || !not_inside_y || !not_inside_z;
}

float BoundBox::getSizeX() const {
    return abs(left_bottom_back.x - right_upper_front.x);
}

float BoundBox::getSizeY() const {
    return abs(left_bottom_back.y - right_upper_front.y);
}

float BoundBox::getSizeZ() const {
    return abs(left_bottom_back.z - right_upper_front.z);
}

glm::vec3 BoundBox::getLeftBottomBack() const {
    return left_bottom_back;
}

glm::vec3 BoundBox::getRightBottomBack() const {
    return left_bottom_back + vec3(getSizeX(), 0, 0);
}

glm::vec3 BoundBox::getRightBottomFront() const {
    return left_bottom_back + vec3(getSizeX(), 0, getSizeZ());
}

glm::vec3 BoundBox::getLeftBottomFront() const {
    return left_bottom_back + vec3(0, 0, getSizeZ());
}

glm::vec3 BoundBox::getLeftUpperBack() const {
    return left_bottom_back + vec3(0, getSizeY(), 0);
}

glm::vec3 BoundBox::getRightUpperBack() const {
    return left_bottom_back + vec3(getSizeX(), getSizeY(), 0);
}

glm::vec3 BoundBox::getRightUpperFront() const {
    return right_upper_front;
}

glm::vec3 BoundBox::getLeftUpperFront() const {
    return left_bottom_back + vec3(0, getSizeY(), getSizeZ());
}


BoundBox::BoundBox(const glm::vec3 &left_bottom_back, const glm::vec3 &right_upper_front)
        : left_bottom_back(left_bottom_back), right_upper_front(right_upper_front) {

}
