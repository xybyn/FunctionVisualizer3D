#ifndef OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H
#define OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H

#include "common/WorldObject.h"
#include "common/trees/CubeTree.h"
#include "common/abstracts/BoundBox.h"
typedef float (*DoubleVariableFunction)(float, float);
class ParametricFunctionDrawer : public WorldObject
{
public:
    CubeTree<int> *tree;
    BoundBox *bb = nullptr;
public:
    ParametricFunctionDrawer(DoubleVariableFunction x,DoubleVariableFunction y, DoubleVariableFunction z,
                             const glm::vec2 &u_range, const glm::vec2 &v_range, const glm::ivec2 &divisions, bool inverted = false);
};

#endif //OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H
