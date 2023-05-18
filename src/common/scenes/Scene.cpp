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

void Scene::removeAll()
{
	for (auto it = objects.begin(); it!=objects.end();it++)
	{
		delete (*it);
	}

	objects.clear();
}
