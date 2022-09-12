//
// Created by pc on 02.08.22.
//

#include "MaterialsScene.h"
#include "common/shaders/AmbientDiffuseSpecularShader.h"
#include "common/primitives/Sphere.h"
#include <vector>

using namespace std;
using namespace glm;

void MaterialsScene::update(float dt) {
    Scene::update(dt);
    static AmbientDiffuseSpecularShader::Light light;
    static AmbientDiffuseSpecularShader::Material material;

    //ImGui::Text("Light");
    //ImGui::SliderFloat4("Position", &light.position.x, -10, 10);
    //ImGui::SliderFloat3("La", &light.La.x, 0, 1);
    //ImGui::SliderFloat3("Ld", &light.Ld.x, 0, 1);
    //ImGui::SliderFloat3("Ls", &light.Ls.x, 0, 1);
    //ImGui::Separator();
    //ImGui::Text("Material");
    //ImGui::SliderFloat("Shininess", &material.shininess, 0, 1024);
    //ImGui::SliderFloat3("Ka", &material.Ka.x, 0, 1);
    //ImGui::SliderFloat3("Kd", &material.Kd.x, 0, 1);
    //ImGui::SliderFloat3("Ks", &material.Ks.x, 0, 1);

    vector<const char *> material_names = {
            "emerald",
            "jade",
            "obsidian",
            "pearl",
            "ruby",
            "turquoise",
            "brass",
            "bronze",
            "chrome",
            "copper",
            "silver",
            "black plastic",
            "cyan plastic",
            "green plastic",
            "red plastic",
            "white plastic",
            "yellow plastic",
            "black rubber",
            "cyan rubber",
            "green rubber",
            "red rubber",
            "white rubber",
            "yellow rubber"
    };
    vector<AmbientDiffuseSpecularShader::Material> materials = {
            {vec3(0.0215, 0.1745, 0.0215), vec3(0.07568, 0.61424, 0.07568), vec3(0.633, 0.727811, 0.633),
                                                                                                                0.6 *
                                                                                                                128},// emerald",
            {vec3(0.0215, 0.1745, 0.0215), vec3(0.07568, 0.61424, 0.07568), vec3(0.633, 0.727811, 0.633),       0.6 *
                                                                                                                128},// jade",
            {vec3(),                       vec3(),                          vec3(),                             1},// obsidian",
            {vec3(0.25, 0.20725, 0.20725), vec3(1, 0.829, 0.829),           vec3(0.296648, 0.296648, 0.296648), 0.088 *
                                                                                                                128},// pearl",
            {vec3(),                       vec3(),                          vec3(),                             1},// ruby",
            {vec3(),                       vec3(),                          vec3(),                             1},// turquoise",
            {vec3(),                       vec3(),                          vec3(),                             1},// brass",
            {vec3(),                       vec3(),                          vec3(),                             1},// bronze",
            {vec3(),                       vec3(),                          vec3(),                             1},// chrome",
            {vec3(),                       vec3(),                          vec3(),                             1},// copper",
            {vec3(),                       vec3(),                          vec3(),                             1},// silver",
            {vec3(),                       vec3(),                          vec3(),                             1},// black plastic",
            {vec3(),                       vec3(),                          vec3(),                             1},// cyan plastic",
            {vec3(),                       vec3(),                          vec3(),                             1},// green plastic",
            {vec3(),                       vec3(),                          vec3(),                             1},// red plastic",
            {vec3(),                       vec3(),                          vec3(),                             1},// white plastic",
            {vec3(),                       vec3(),                          vec3(),                             1},// yellow plastic"
            {vec3(),                       vec3(),                          vec3(),                             1},// black rubber",
            {vec3(),                       vec3(),                          vec3(),                             1},// cyan rubber",
            {vec3(),                       vec3(),                          vec3(),                             1},// green rubber",
            {vec3(),                       vec3(),                          vec3(),                             1},// red rubber",
            {vec3(),                       vec3(),                          vec3(),                             1},// white rubber",
            {vec3(0.05, 0.05, 0.0),        vec3(0.5, 0.5, 0.4),             vec3(0.7, 0.7, 0.04),               128 *
                                                                                                                0.078125},// yellow rubber",
    };

    static int selected_material_index = 0;
    ImGui::Combo("Select material", &selected_material_index, &material_names[0], material_names.size());
    material = materials[selected_material_index];

    shader->setLight(light);
    shader->setMaterial(material);
}

MaterialsScene::MaterialsScene(CameraBase *camera) : Scene(camera) {
    auto sphere = new Sphere(1.0f);
    add(sphere);
    shader = new AmbientDiffuseSpecularShader(camera);
    sphere->setShader(shader);
}
