//
// Created by pc on 01.08.22.
//

#ifndef OPENGLPROJECT_DISCARDWITHPERLINSHADER_H
#define OPENGLPROJECT_DISCARDWITHPERLINSHADER_H

#include "AmbientDiffuseSpecularShader.h"

class DiscardWithPerlinShader : public AmbientDiffuseSpecularShader
{
public:
    DiscardWithPerlinShader(CameraBase *camera) : AmbientDiffuseSpecularShader(camera, "../src/shaders/discard.vert", "../src/shaders/discard.frag"){}
    void setDiscardStrength(float value) {
        discard_strength = value;
    }
    void use() override{
        AmbientDiffuseSpecularShader::use();
        setFloat("uniform_discard_strength", discard_strength);
    }
private:
    float discard_strength = 0.0f;
};

#endif //OPENGLPROJECT_DISCARDWITHPERLINSHADER_H
