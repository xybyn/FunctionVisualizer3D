#ifndef OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H
#define OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H

#include "common/WorldObject.h"
#include "common/abstracts/BoundBox.h"
typedef float (*SDF)(const glm::vec3 &);
class ImplicitFunctionDrawer : public WorldObject
{
public:
    ImplicitFunctionDrawer(SDF function, const glm::vec3 &step, const BoundBox &bound, bool inverted = false);

private:

    struct ThreadResult{
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<uint> indices;

    };

    void calculate_parallel(int count_of_threads, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<uint> &indices,
                            const glm::vec3 &step, const BoundBox &bound, SDF function);

    void get_volume_vertices_normals_indices(std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<uint> &indices,
                                             const glm::vec3 &step, BoundBox bound, SDF function);
};

#endif //OPENGLPROJECT_BOUNDBOX_H
