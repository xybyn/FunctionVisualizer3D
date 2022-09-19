#ifndef OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H
#define OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H


#include "Scene.h"
#include "common/shaders/DefaultShader.h"
#include "common/shaders/AmbientDiffuseSpecularShader.h"
#include "common/function_drawers/ImplicitFunctionDrawer.h"
#include "common/function_drawers/ParametricFunctionDrawer.h"
#include "common/shaders/FlatADSShader.h"
#include "common/primitives/BoundBoxRenderer.h"
#include "common/abstracts/BoundBox.h"
class FunctionVisualizerScene : public Scene {
public:
    FunctionVisualizerScene(CameraBase *camera);
    void update(float dt) override;

    std::string getName() const override { return "FunctionVisualizerScene"; }
private:
    BoundBoxRenderer *bb = nullptr;
    DefaultShader *bb_shader = nullptr;
    ImplicitFunctionDrawer*implicit_function_drawer = nullptr;
    ParametricFunctionDrawer *parametric_function_drawer = nullptr;
    AmbientDiffuseSpecularShader *function_shader = nullptr;
    FlatADSShader *flat_shader = nullptr;
};


#endif //OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H
