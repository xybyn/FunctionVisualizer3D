#ifndef OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H
#define OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H

#include "common/WorldObject.h"
#include "common/abstracts/BoundBox.h"

typedef float (*SDF)(const glm::vec3 &);

class ImplicitFunctionDrawerChunk : public WorldObject
{
public:
    ImplicitFunctionDrawerChunk(SDF function, const glm::vec3 &step, const BoundBox &bound, bool inverted = false);

    void get_volume_vertices_normals_indices();

    const BoundBox &bound;
    SDF function;
    const glm::vec3 &step;
};

class ImplicitFunctionDrawer : public WorldObject
{
public:
    ImplicitFunctionDrawer(SDF function, const glm::vec3 &step, const BoundBox &bound, bool inverted = false);
    void setShader(Shader *shader) override;
    void render() override;

private:
    std::vector<ImplicitFunctionDrawerChunk *> chunks;
    void calculate_parallel(int count_of_threads, const glm::vec3 &step, const BoundBox &bound, SDF function);
};

#endif //OPENGLPROJECT_BOUNDBOX_H
