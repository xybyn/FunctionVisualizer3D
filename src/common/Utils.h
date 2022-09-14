//
// Created by pc on 14.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_UTILS_H
#define FUNCTIONVISUALIZER3D_UTILS_H
#include <glm/common.hpp>
inline bool equals(float a, float b, float eps) {
    return fabsf(a - b) <= eps;
}

inline bool less_or_equals(float a, float b, float eps) {
    bool are_equals = equals(a, b, eps);
    if (are_equals)
        return true;
    return a < b;
}

inline bool in_range(float x, float a, float b, float eps) {
    return less_or_equals(a, x, eps) && less_or_equals(x, b, eps);
}

inline bool are_points_same(const glm::vec3 &a, const glm::vec3 &b)
{
    float eps = 0.00001f;
    return equals(a.x, b.x, eps) && equals(a.y, b.y, eps) && equals(a.z, b.z, eps);
}
#endif //FUNCTIONVISUALIZER3D_UTILS_H
