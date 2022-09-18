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

class Scene {
public:
    Scene(CameraBase *camera) : camera(camera){ }
    void add(WorldObject *object) { objects.push_back(object); }

    virtual void update(float dt) { camera->update(dt);
        for (int i = 0; i < objects.size(); ++i)
        {
            objects[i]->update(dt);
        }
    }

    virtual void render();

    virtual std::string getName() const = 0;
protected:
    CameraBase *camera;
private:
    std::vector<WorldObject *> objects;

};


#endif //OPENGLPROJECT_SCENE_H
