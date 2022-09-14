//
// Created by pc on 01.08.22.
//

#ifndef OPENGLPROJECT_FLATADSSHADER_H
#define OPENGLPROJECT_FLATADSSHADER_H

#include "AmbientDiffuseSpecularShader.h"
#include "DefaultShader.h"
#include "RootDir.h"
class FlatADSShader : public AmbientDiffuseSpecularShader
{
public:
    FlatADSShader(CameraBase *camera) : AmbientDiffuseSpecularShader(camera, ROOT_DIR "src/shaders/flat_ads.vert", ROOT_DIR "src/shaders/flat_ads.frag") {}
};

#endif //OPENGLPROJECT_FLATADSSHADER_H
