//
// Created by pc on 02.08.22.
//

#ifndef OPENGLPROJECT_RAYMARCHINGSCENE_H
#define OPENGLPROJECT_RAYMARCHINGSCENE_H


#include "Scene.h"
#include "common/shaders/AmbientDiffuseSpecularShader.h"
#include "common/shaders/DefaultShader.h"
#include "common/shaders/ComputeShader.h"
class RaymarchingScene : public Scene {
public:
    RaymarchingScene(CameraBase* camera);
    void update(float dt) override;

    std::string getName() const override { return "RaymarchingScene"; }
private:
    Shader* shader = nullptr;
    ComputeShader* compute_shader = nullptr;
};


#endif //OPENGLPROJECT_MATERIALSSCENE_H
