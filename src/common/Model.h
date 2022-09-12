/** 
 * Code taken from www.learnopengl.com
 * Slightly modified by Tomasz Gałaj 2018
 **/

#ifndef MODEL_H
#define MODEL_H

#include <gl/glew.h>


#include <vector>
#include "Mesh.h"
#include "assimp/scene.h"
#include "WorldObject.h"

class Model : public WorldObject {
public:
    std::vector<Mesh> meshes;
    std::string directory;

    Model(const std::string &path);

    void render() override;

private:
    void load_model(const std::string &path);

    void process_node(aiNode *node, const aiScene *scene);

    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
};

#endif