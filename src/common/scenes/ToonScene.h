//
// Created by pc on 02.08.22.
//

#ifndef OPENGLPROJECT_TOONSCENE_H
#define OPENGLPROJECT_TOONSCENE_H


#include "Scene.h"
#include "common/shaders/ToonADSShader.h"

class ToonScene : public Scene {
public:
    void update(float dt) override;

    ToonScene(CameraBase *camera);

    std::string getName() const override { return "Toon"; }
private:
    ToonADSShader *toon_shader = nullptr;
};


#endif //OPENGLPROJECT_TOONSCENE_H
