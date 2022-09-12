#include "Mesh.h"

Mesh::Mesh(const std::vector<glm::vec3> &vertices, const std::vector<glm::vec3> &normals,
           const std::vector<glm::vec2> &tex_coord, const std::vector<glm::vec3> &tangents, const std::vector<unsigned int> &indices){
    this->vertices = vertices;
    this->normals = normals;
    this->tex_coords = tex_coords;
    this->indices = indices;
    this->tangents = tangents;
    initialize_buffers();
}
