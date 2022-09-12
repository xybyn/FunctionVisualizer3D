//
// Created by pc on 27.07.22.
//

#include "DefaultShader.h"

void DefaultShader::use() {
    Shader::use();
    setMat4("view", camera->getView());
    setMat4("projection", camera->getProjection());
}
