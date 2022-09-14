//
// Created by pc on 03.08.22.
//

#include "NormalMapScene.h"
#include "common/primitives/Sphere.h"
#include "common/shaders/NormalMapShader.h"
#include "common/Mesh.h"
#include "common/Model.h"

void NormalMapScene::update(float dt) {
    Scene::update(dt);
}

void NormalMapScene::render() {
    texture->bind(0);
    shader->setInt("tex_sampler_0", 0);
    texture->bind(1);
    shader->setInt("tex_sampler_1", 1);

    Scene::render();
}

NormalMapScene::NormalMapScene(CameraBase *camera) : Scene(camera) {
    auto sphere = new Model(ROOT_DIR "resources/bs_ears.obj");
    add(sphere);
    shader = new NormalMapShader(camera);
    sphere->setShader(shader);
    texture = new Texture();
    texture->load(0,ROOT_DIR "resources/ogre_diffuse.png");
    texture->load(1,ROOT_DIR "resources/ogre_normalmap.png");
}
