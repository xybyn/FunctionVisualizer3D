//
// Created by pc on 27.07.22.
//

#include "WorldObject.h"

void WorldObject::render() {
    if(shader)
        shader->use();
    if(texture)
        texture->bind();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    if(!normals_shader)
        return;
    normals_shader->use();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void WorldObject::setWorldPosition(const glm::vec3 &new_world_position) {

}

WorldObject::WorldObject() {
}

void WorldObject::initialize_buffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (vertices.size()* 3 + normals.size()* 3+ tex_coords.size()* 2) * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float) * 3, &vertices[0]);

    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, normals.size() * sizeof(float) * 3,
                    &normals[0]);

    glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size())* sizeof(float) * 3, tex_coords.size() * sizeof(float) * 2,
                    &tex_coords[0]);
    if(!tangents.empty())
    glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size())* sizeof(float) * 3 + tex_coords.size()*sizeof(float)*2, tangents.size() * sizeof(float) * 3,
                    &tangents[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void *) (vertices.size() * sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 2 * sizeof(float), (void *) ((vertices.size() + normals.size()) * sizeof(float) * 3));
    glEnableVertexAttribArray(2);
    if(!tangents.empty())
    {
        glVertexAttribPointer(3, 3, GL_FLOAT, false, 3 * sizeof(float), (void *) ((vertices.size() + normals.size()) * sizeof(float) * 3 + tex_coords.size()*sizeof(float)*2));
        glEnableVertexAttribArray(3);
    }


    glBindVertexArray(0);
}
