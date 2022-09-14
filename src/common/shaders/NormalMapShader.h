//
// Created by pc on 03.08.22.
//

#ifndef OPENGLPROJECT_NORMALMAPSHADER_H
#define OPENGLPROJECT_NORMALMAPSHADER_H
#include "AmbientDiffuseSpecularShader.h"
#include "RootDir.h"
class NormalMapShader : public AmbientDiffuseSpecularShader
{
public:
    NormalMapShader(CameraBase *camera) : AmbientDiffuseSpecularShader(camera, ROOT_DIR "src/shaders/normal_map.vert", ROOT_DIR "src/shaders/normal_map.frag") {}

};
#endif //OPENGLPROJECT_NORMALMAPSHADER_H
