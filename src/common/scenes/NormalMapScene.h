//
// Created by pc on 03.08.22.
//

#ifndef OPENGLPROJECT_NORMALMAPSCENE_H
#define OPENGLPROJECT_NORMALMAPSCENE_H


#include "Scene.h"

class NormalMapScene : public Scene {
public:
    NormalMapScene(CameraBase *camera);
    std::string getName() const override { return "Normal Map"; }

    void update(float dt) override;

    void render() override;
private:
    Shader *shader = nullptr;
    Texture *texture = nullptr;
};


#endif //OPENGLPROJECT_NORMALMAPSCENE_H
