//
// Created by pc on 02.08.22.
//

#ifndef OPENGLPROJECT_NURBSSCENE_H
#define OPENGLPROJECT_NURBSSCENE_H


#include "Scene.h"
#include "common/shaders/ToonADSShader.h"

class NURBS
{
public:
    void setPoints(const std::vector<glm::vec4>& ps, const std::vector<float>& ts)
    {
        this->pss = ps;
        this->tss = ts;
    }
    glm::vec3 getPoint(float u);
private:
    float B(float x, int i, int p);
    std::vector<glm::vec4> pss;
    std::vector<float> tss;
    float ts(int i)
    {
        return tss[i];
    }
    glm::vec4 ps(int i)
    {
        return pss[i];
    }
    float ws(int i)
    {
        return pss[i].w;
    }
};

class NURBSSurface
{

};

class NURBSScene : public Scene {
public:
    void update(float dt) override;

    NURBSScene(CameraBase *camera);

    std::string getName() const override { return "Nurbs"; }
    class ParametricFunctionDrawer* parametric_function = nullptr;

    glm::vec3 func(const glm::vec2& uv);
};


#endif //OPENGLPROJECT_TOONSCENE_H
