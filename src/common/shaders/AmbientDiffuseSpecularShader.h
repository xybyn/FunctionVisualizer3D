//
// Created by pc on 27.07.22.
//

#ifndef OPENGLPROJECT_AMBIENTDIFFUSESPECULARSHADER_H
#define OPENGLPROJECT_AMBIENTDIFFUSESPECULARSHADER_H


#include "DefaultShader.h"
#include "RootDir.h"
class AmbientDiffuseSpecularShader : public DefaultShader {
public:
    AmbientDiffuseSpecularShader(CameraBase *camera) :
            DefaultShader(camera, ROOT_DIR "src/shaders/ambient_diffuse_specular.vert",
                          ROOT_DIR "/src/shaders/ambient_diffuse_specular.frag") {}
    AmbientDiffuseSpecularShader(CameraBase *camera, const char *vert, const char *frag) :
            DefaultShader(camera, vert,frag) {}
    void use() override;


    struct Light {
        glm::vec4 position = glm::vec4(0, 4, 4, 1);
        glm::vec3 La= glm::vec3(1.0);
        glm::vec3 Ld= glm::vec3(1.0);
        glm::vec3 Ls= glm::vec3(1.0);
    };
    struct Material {
        glm::vec3 Ka= glm::vec3(0.5);
        glm::vec3 Kd= glm::vec3(0.5);
        glm::vec3 Ks= glm::vec3(0.5);
        float shininess = 32.0f;
    };
    void setLight(const Light &light){
        this->light = light;
    }
    void setMaterial(const Material &material){
        this->material = material;
    }
    const Light & getLight(){
        return light;
    }
    const Material & getMaterial(){
        return material;
    }
private:
    Light light;
    Material material;
};


#endif //OPENGLPROJECT_AMBIENTDIFFUSESPECULARSHADER_H
