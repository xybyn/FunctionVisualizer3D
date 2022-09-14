//
// Created by pc on 27.07.22.
//

#ifndef OPENGLPROJECT_NORMALSSHADER_H
#define OPENGLPROJECT_NORMALSSHADER_H


#include "DefaultShader.h"
#include "RootDir.h"
class NormalsShader : public DefaultShader {
public:
    NormalsShader(CameraBase *camera) :
            DefaultShader(camera, ROOT_DIR "src/shaders/normals.vert",
                          ROOT_DIR "/src/shaders/normals.frag",
                          ROOT_DIR "src/shaders/normals.geom") {}
    NormalsShader(CameraBase *camera, const char *vert, const char *frag) :
            DefaultShader(camera, vert,frag) {}
    void use() override;
};


#endif //OPENGLPROJECT_NORMALSSHADER_H
