//
// Created by pc on 03.08.22.
//

#include "MultipleTexturesScene.h"
#include "common/primitives/Sphere.h"
#include "common/shaders/MultipleTexturesShader.h"

void MultipleTexturesScene::update(float dt) {
    Scene::update(dt);
}

void MultipleTexturesScene::render() {
    texture->bind(0);
    shader->setInt("tex_sampler_0", 0);
    texture->bind(1);
    shader->setInt("tex_sampler_1", 1);

    Scene::render();
}

MultipleTexturesScene::MultipleTexturesScene(CameraBase *camera) : Scene(camera) {
    auto sphere = new Sphere(1.0f);
    add(sphere);
    shader = new MultipleTexturesShader(camera);
    sphere->setShader(shader);
    texture = new Texture();
    texture->load(0, ROOT_DIR "resources/moss.png");
    texture->load(1, ROOT_DIR "resources/brick1.jpg");
}
