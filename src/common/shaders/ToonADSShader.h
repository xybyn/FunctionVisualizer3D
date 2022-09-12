//
// Created by pc on 02.08.22.
//

#ifndef OPENGLPROJECT_TOONADSSHADER_H
#define OPENGLPROJECT_TOONADSSHADER_H

#include "AmbientDiffuseSpecularShader.h"
#include "DefaultShader.h"
#include "RootDir.h"
class ToonADSShader : public AmbientDiffuseSpecularShader
{
public:
    ToonADSShader(CameraBase *camera) : AmbientDiffuseSpecularShader(camera, ROOT_DIR "src/shaders/toon.vert",  ROOT_DIR "src/shaders/toon.frag") {}
    void setLevels(int levels) { this->levels = levels;}
    void setScaleFactor(float factor) { this->scale_factor = factor;}
    void use() override{
        AmbientDiffuseSpecularShader::use();
        setInt("uniform_levels", levels);
        setFloat("uniform_scale_factor", scale_factor);
    }
private:
    int levels = 3;
    float scale_factor = 1;
};


#endif //OPENGLPROJECT_TOONADSSHADER_H
