//
// Created by pc on 03.08.22.
//

#ifndef OPENGLPROJECT_MULTIPLETEXTURESSCENE_H
#define OPENGLPROJECT_MULTIPLETEXTURESSCENE_H


#include "Scene.h"

class MultipleTexturesScene : public Scene {
public:
    MultipleTexturesScene(CameraBase *camera);
    std::string getName() const override { return "Multiple Textures"; }

    void update(float dt) override;

    void render() override;
private:
    Shader *shader = nullptr;
    Texture *texture = nullptr;
};


#endif //OPENGLPROJECT_MULTIPLETEXTURESSCENE_H
