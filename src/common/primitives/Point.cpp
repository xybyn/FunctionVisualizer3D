//
// Created by pc on 14.09.22.
//

#include "Point.h"

Point::Point(const glm::vec3 &position) {
    vertices.push_back(position);
    indices.push_back(0);
    initialize_buffers();
}

void Point::render() {
    if(shader)
        shader->use();
    if(texture)
        texture->bind();
    glBindVertexArray(VAO);
    glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
}
