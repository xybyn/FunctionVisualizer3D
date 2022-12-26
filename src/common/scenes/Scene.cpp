//
// Created by pc on 02.08.22.
//

#include "Scene.h"

void Scene::render() {
	for (auto& it = objects.begin(); it != objects.end(); it++)
	{
		(*it)->render();
	}
}
