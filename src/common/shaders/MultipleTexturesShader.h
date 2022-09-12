//
// Created by pc on 03.08.22.
//

#ifndef OPENGLPROJECT_MULTIPLETEXTURESSHADER_H
#define OPENGLPROJECT_MULTIPLETEXTURESSHADER_H

#include "AmbientDiffuseSpecularShader.h"

class MultipleTexturesShader : public AmbientDiffuseSpecularShader
{
public:
    MultipleTexturesShader(CameraBase *camera) : AmbientDiffuseSpecularShader(camera, "../src/shaders/multi_textures.vert", "../src/shaders/multi_textures.frag") {}

};
#endif //OPENGLPROJECT_MULTIPLETEXTURESSHADER_H
