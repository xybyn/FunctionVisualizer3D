//
// Created by pc on 02.08.22.
//

#include "RaymarchingScene.h"
#include "common/primitives/FullscreenRectangle.h"
using namespace std;
using namespace glm;

void RaymarchingScene::update(float dt)
{
    Scene::update(dt);
 
}

RaymarchingScene::RaymarchingScene(CameraBase* camera) : Scene(camera)
{
    shader = new Shader(ROOT_DIR "src/shaders/raymarching.vert", ROOT_DIR "src/shaders/raymarching.frag");

    compute_shader = new ComputeShader(ROOT_DIR "src/shaders/raymarching.comp");
    FullscreenRectangle* rect = new FullscreenRectangle();
    rect->setShader(shader);
    add(rect);
}
