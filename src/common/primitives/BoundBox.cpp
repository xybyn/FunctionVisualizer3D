#include "BoundBox.h"
using namespace glm;

float BoundBox::getSizeX() const
{
    return fabsf(left_lower_back.x - right_upper_front.x);
}

float BoundBox::getSizeY() const
{
    return fabsf(left_lower_back.y - right_upper_front.y);
}
float BoundBox::getSizeZ() const
{
    return fabsf(left_lower_back.z - right_upper_front.z);
}

void BoundBox::render()
{
    if (shader)
        shader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

BoundBox::BoundBox(const glm::vec3 &left_lower_back, const glm::vec3 &right_upper_front)
    : left_lower_back(left_lower_back), right_upper_front(right_upper_front)
{

    float size_x = getSizeX();
    float size_y = getSizeY();
    float size_z = getSizeZ();

    vertices.push_back(left_lower_back + vec3(0, 0, 0));
    vertices.push_back(left_lower_back + vec3(size_x, 0, 0));
    vertices.push_back(left_lower_back + vec3(size_x, 0, size_z));
    vertices.push_back(left_lower_back + vec3(0, 0, size_z));

    vertices.push_back(left_lower_back + vec3(0, size_y, 0));
    vertices.push_back(left_lower_back + vec3(size_x, size_y, 0));
    vertices.push_back(left_lower_back + vec3(size_x, size_y, size_z));
    vertices.push_back(left_lower_back + vec3(0, 0, size_z));

    indices.push_back(0);
    indices.push_back(1);

    indices.push_back(1);
    indices.push_back(2);

    indices.push_back(2);
    indices.push_back(3);

    indices.push_back(3);
    indices.push_back(0);

    indices.push_back(4);
    indices.push_back(5);

    indices.push_back(5);
    indices.push_back(6);

    indices.push_back(6);
    indices.push_back(7);

    indices.push_back(7);
    indices.push_back(4);

    indices.push_back(0);
    indices.push_back(4);

    indices.push_back(1);
    indices.push_back(5);

    indices.push_back(2);
    indices.push_back(6);

    indices.push_back(3);
    indices.push_back(7);
    initialize_buffers();
}
