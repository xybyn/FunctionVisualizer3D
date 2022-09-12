#include "FunctionVisualizerScene.h"
#include <vector>
#include "common/primitives/BoundBox.h"
using namespace std;
using namespace glm;

void FunctionVisualizerScene::update(float dt)
{
    Scene::update(dt);
}

FunctionVisualizerScene::FunctionVisualizerScene(CameraBase *camera) : Scene(camera)
{
    auto p0 = vec3(-0.5f);
    auto p1 = vec3(0.5);
    //bb = new BoundBox(p0, p1);
    // add(bb);
}
