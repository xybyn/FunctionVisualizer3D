//
// Created by pc on 27.07.22.
//

#ifndef OPENGLPROJECT_SINGLECOLORSHADER_H
#define OPENGLPROJECT_SINGLECOLORSHADER_H


#include "common/Shader.h"
#include "common/cameras/FreeCamera.h"
#include "DefaultShader.h"
#include "RootDir.h"
class SingleColorShader : public DefaultShader {
public:
    SingleColorShader(CameraBase* camera) :
        DefaultShader(camera, ROOT_DIR "src/shaders/single_color.vert", ROOT_DIR "src/shaders/single_color.frag") {}

    void use() override;

    void setColor(const glm::vec3& color) {
        this->color = color;
    }
private:
    glm::vec3 color = glm::vec3(0);
};


#endif //OPENGLPROJECT_DIFFUSESHADER_H
