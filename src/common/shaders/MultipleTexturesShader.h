//
// Created by pc on 03.08.22.
//

#ifndef OPENGLPROJECT_MULTIPLETEXTURESSHADER_H
#define OPENGLPROJECT_MULTIPLETEXTURESSHADER_H

#include "AmbientDiffuseSpecularShader.h"
#include "RootDir.h"
class MultipleTexturesShader : public AmbientDiffuseSpecularShader
{
public:
    MultipleTexturesShader(CameraBase *camera) : AmbientDiffuseSpecularShader(camera, ROOT_DIR "src/shaders/multi_textures.vert", ROOT_DIR "src/shaders/multi_textures.frag") {}

};
#endif //OPENGLPROJECT_MULTIPLETEXTURESSHADER_H
