#ifndef OPENGLPROJECT_CUBETREERENDERER_H
#define OPENGLPROJECT_CUBETREERENDERER_H

#include <vector>
#include <glm/common.hpp>
#include "common/abstracts/BoundBox.h"
#include "common/WorldObject.h"
#include "common/trees/CubeTree.h"
#include "common/primitives/BoundBoxRenderer.h"
template<typename T>
class CubeTreeRenderer : public WorldObject
{
private:
    CubeTree<T> *tree = nullptr;
    std::vector<BoundBoxRenderer *> renderers;
    void add_renderers_recursive(CubeTree<T> *tree);
public:
    CubeTreeRenderer(CubeTree<T> *tree);
    void render() override;
};

template<typename T>
void CubeTreeRenderer<T>::add_renderers_recursive(CubeTree<T> *tree)
{
    renderers.push_back(new BoundBoxRenderer(tree->getBoundBox()));
    if(tree->isDivided())
    {
        add_renderers_recursive(tree->getLeftBottomBack());
        add_renderers_recursive(tree->getRightBottomBack());
        add_renderers_recursive(tree->getRightBottomFront());
        add_renderers_recursive(tree->getLeftBottomFront());

        add_renderers_recursive(tree->getLeftUpperBack());
        add_renderers_recursive(tree->getRightUpperBack());
        add_renderers_recursive(tree->getRightUpperFront());
        add_renderers_recursive(tree->getLeftUpperFront());
    }
}

template<typename T>
CubeTreeRenderer<T>::CubeTreeRenderer(CubeTree<T> *tree) : tree(tree)
{
    add_renderers_recursive(tree);
}

template<typename T>
void CubeTreeRenderer<T>::render()
{
    if(shader)
        shader->use();
    for(int i = 0; i < renderers.size(); i++)
        renderers[i]->render();
}
#endif // OPENGLPROJECT_CUBETREERENDERER_H
