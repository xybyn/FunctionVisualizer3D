/**
* Code taken from www.learnopengl.com
* Slightly modified by Tomasz Gałaj 2018
**/

#ifndef MESH_H
#define MESH_H

//#include <glad/glad.h> // holds all OpenGL type declarations
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "WorldObject.h"

class Mesh : public WorldObject
{
public:
    /*  Functions  */
    // constructor
    Mesh(const std::vector<glm::vec3> &vertices,
         const std::vector<glm::vec3> &normals,
         const std::vector<glm::vec2> &tex_coords,
         const std::vector<glm::vec3> &tangents,
         const std::vector<unsigned int> &indices);


private:

};
#endif