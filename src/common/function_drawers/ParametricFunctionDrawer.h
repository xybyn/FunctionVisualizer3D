#ifndef OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H
#define OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H

#include <thread>
#include "common/WorldObject.h"
#include "common/trees/CubeTree.h"
#include "common/abstracts/BoundBox.h"
#include "common/tasks/ParallelTask.h"

typedef glm::vec3 (*DoubleVariableFunction)(float, float);

class ParametricFunctionDrawer : public WorldObject
{
public:
    CubeTree<int> *tree;
    BoundBox *bb = nullptr;

    void update(float dt) override;

    void render() override;

public:
    ParametricFunctionDrawer(DoubleVariableFunction func, const glm::vec2 &u_range, const glm::vec2 &v_range,
                             const glm::ivec2 &divisions, bool inverted = false);


private:
    glm::vec2 u_range;
    glm::vec2 v_range;
    glm::ivec2 divisions;
    bool inverted;
    DoubleVariableFunction func;
    class ParametricFunctionDrawerTask : public ParallelTask<int>
    {
    public:
        ParametricFunctionDrawerTask(ParametricFunctionDrawer *owner);

        void run() override;

        ~ParametricFunctionDrawerTask()
        {
            t->join();
            delete t;
        }

    private:
        std::thread *t = nullptr;

        void process(ParametricFunctionDrawerTask *owner);

        ParametricFunctionDrawer *owner = nullptr;
    } *task = nullptr;
    friend  class ParametricFunctionDrawerTask;
};

#endif //OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H
