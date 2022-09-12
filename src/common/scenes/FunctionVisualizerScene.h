#ifndef OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H
#define OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H


#include "Scene.h"
//#include "common/shaders/AmbientDiffuseSpecularShader.h"
class FunctionVisualizerScene : public Scene {
public:
    FunctionVisualizerScene(CameraBase *camera);
    void update(float dt) override;

    std::string getName() const override { return "FunctionVisualizerScene"; }
private:
    class BoundBox *bb = nullptr;
};


#endif //OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H
