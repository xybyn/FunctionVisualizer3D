#include "BoundBoxRenderer.h"
using namespace glm;

void BoundBoxRenderer::render()
{
    if (shader)
        shader->use();
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

BoundBoxRenderer::BoundBoxRenderer(const BoundBox &bb) : bb(bb)
{
    float size_x = bb.getSizeX();
    float size_y = bb.getSizeY();
    float size_z = bb.getSizeZ();

    vertices.push_back(bb.getLeftBottomBack());
    vertices.push_back(bb.getRightBottomBack());
    vertices.push_back(bb.getRightBottomFront());
    vertices.push_back(bb.getLeftBottomFront());

    vertices.push_back(bb.getLeftUpperBack());
    vertices.push_back(bb.getRightUpperBack());
    vertices.push_back(bb.getRightUpperFront());
    vertices.push_back(bb.getLeftUpperFront());

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
    WorldObject::initialize_buffers();
}
