//
// Created by pc on 02.08.22.
//

#include "Scene.h"

void Scene::render() {
    for (int i = 0; i < objects.size(); ++i) {
        objects[i]->render();
    }
}
