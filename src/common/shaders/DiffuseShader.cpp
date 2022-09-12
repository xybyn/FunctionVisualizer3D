//
// Created by pc on 27.07.22.
//

#include "DiffuseShader.h"
void DiffuseShader::use() {
    DefaultShader::use();
    setVec3("uniform_Kd", Kd);
    setVec3("uniform_Ld", Ld);
    setVec4("uniform_light_position", light_position);
}
