#ifndef OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H
#define OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H


#include "Scene.h"
#include "common/shaders/DefaultShader.h"
#include "common/shaders/AmbientDiffuseSpecularShader.h"
#include "common/shaders/SingleColorShader.h"
#include "common/function_drawers/ImplicitFunctionDrawer.h"
#include "common/function_drawers/ParametricFunctionDrawer.h"
#include "common/shaders/FlatADSShader.h"
#include "common/shaders/SingleColorShader.h"
#include "common/primitives/BoundBoxRenderer.h"
#include "common/primitives/Plane.h"
#include "common/abstracts/BoundBox.h"
class FunctionVisualizerScene : public Scene {
public:
	FunctionVisualizerScene(CameraBase* camera);
	void update(float dt) override;

	std::string getName() const override { return "FunctionVisualizerScene"; }
private:
	WorldObject* object = nullptr;
	AmbientDiffuseSpecularShader* function_shader = nullptr;

	SingleColorShader* light_shader = nullptr;

	SingleColorShader* red = nullptr;

	FlatADSShader* flat_shader = nullptr;
	void on_progress(float progress);
	Plane* plane = nullptr;
	float f(const  glm::vec3& v);
};


#endif //OPENGLPROJECT_FUNCTIONVISUALIZERSCENE_H
