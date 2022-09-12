//
// Created by pc on 27.07.22.
//

#include "AmbientDiffuseSpecularShader.h"

void AmbientDiffuseSpecularShader::use() {
    DefaultShader::use();
    setVec4("uniform_light.position",light.position);
    setVec3("uniform_light.La",light.La);
    setVec3("uniform_light.Ld",light.Ld);
    setVec3("uniform_light.Ls",light.Ls);

    setVec3("uniform_material.Ka",material.Ka);
    setVec3("uniform_material.Kd",material.Kd);
    setVec3("uniform_material.Ks",material.Ks);
    setFloat("uniform_material.shininess",material.shininess);
}
