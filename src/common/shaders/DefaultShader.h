//
// Created by pc on 27.07.22.
//

#ifndef OPENGLPROJECT_DEFAULTSHADER_H
#define OPENGLPROJECT_DEFAULTSHADER_H

#include "../Shader.h"
#include "common/cameras/CameraBase.h"

class DefaultShader : public Shader {
public:
    DefaultShader(CameraBase * camera) :
    Shader("../src/shaders/default.vert", "../src/shaders/default.frag"),
    camera(camera){}

    DefaultShader(CameraBase * camera, const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr) :
            Shader(vertexPath, fragmentPath, geometryPath),
            camera(camera){}
    void use() override;
private:
    CameraBase *camera;
};


#endif //OPENGLPROJECT_DEFAULTSHADER_H
