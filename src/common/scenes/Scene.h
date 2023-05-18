//
// Created by pc on 02.08.22.
//

#ifndef OPENGLPROJECT_SCENE_H
#define OPENGLPROJECT_SCENE_H


#include "common/WorldObject.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "common/cameras/CameraBase.h"
#include <list>

class Scene {
public:
	Scene(CameraBase* camera) : camera(camera) { }
	void add(WorldObject* object) { objects.push_back(object); }
	void remove(WorldObject* object) { objects.remove(object); }

	virtual void update(float dt) {
		camera->update(dt);
		for (auto& it = objects.begin(); it != objects.end(); it++)
		{
			(*it)->update(dt);
		}
	}

	virtual void render();

	virtual std::string getName() const = 0;
protected:
	CameraBase* camera;
	void removeAll();
	std::list<WorldObject*> objects;

};


#endif //OPENGLPROJECT_SCENE_H
