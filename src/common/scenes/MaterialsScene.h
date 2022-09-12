//
// Created by pc on 02.08.22.
//

#ifndef OPENGLPROJECT_MATERIALSSCENE_H
#define OPENGLPROJECT_MATERIALSSCENE_H


#include "Scene.h"
#include "common/shaders/AmbientDiffuseSpecularShader.h"

class MaterialsScene : public Scene {
public:
    MaterialsScene(CameraBase *camera);
    void update(float dt) override;

    std::string getName() const override { return "Materials"; }
private:
    AmbientDiffuseSpecularShader *shader = nullptr;
};


#endif //OPENGLPROJECT_MATERIALSSCENE_H
