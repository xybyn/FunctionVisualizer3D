//
// Created by pc on 02.08.22.
//

#include "ToonScene.h"
#include "common/primitives/Sphere.h"

void ToonScene::update(float dt) {
    Scene::update(dt);
    static float scale_factor = 0.4;
    static int levels = 3;
    ImGui::SliderInt("Levels", &levels, 0, 10);
    ImGui::SliderFloat("Scale Factor", &scale_factor, 0, 1);
    toon_shader->setLevels(levels);
    toon_shader->setScaleFactor(scale_factor);
}

ToonScene::ToonScene(CameraBase *camera) : Scene(camera) {
    auto sphere = new Sphere(1, 40, 40);
    add(sphere);
    toon_shader = new ToonADSShader(camera);
    sphere->setShader(toon_shader);
}
