//
// Created by pc on 27.07.22.
//

#ifndef OPENGLPROJECT_DIFFUSESHADER_H
#define OPENGLPROJECT_DIFFUSESHADER_H


#include "common/Shader.h"
#include "common/cameras/FreeCamera.h"
#include "DefaultShader.h"
#include "RootDir.h"
class DiffuseShader : public DefaultShader {
public:
    DiffuseShader(CameraBase *camera) :
            DefaultShader(camera, ROOT_DIR "src/shaders/diffuse.vert", ROOT_DIR "src/shaders/diffuse.frag"){}

    void use() override;

    void setKd(const glm::vec3 &Kd) {
        this->Kd = Kd;
    }

    void setLd(const glm::vec3 &Ld) {
        this->Ld = Ld;
    }

    void setLightPosition(const glm::vec4 &light_position) {
        this->light_position = light_position;
    }

private:
    glm::vec3 Kd;
    glm::vec3 Ld;
    glm::vec4 light_position;
};


#endif //OPENGLPROJECT_DIFFUSESHADER_H
