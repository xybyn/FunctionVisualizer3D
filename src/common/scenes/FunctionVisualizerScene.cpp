#include "FunctionVisualizerScene.h"
#include <vector>
#include "common/primitives/BoundBoxRenderer.h"
#include "common/trees/CubeTree.h"
#include "common/trees/CubeTreeRenderer.h"
#include <common/primitives/Sphere.h>
#include "common/Texture.h"
#include "common/primitives/Point.h"
#include "common/shaders/NormalsShader.h"
#include "common/primitives/Plane.h"
#include "common/tasks/HelloWorldTask.h"
using namespace std;
using namespace glm;


float f0(const vec3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;

	//return sin(x*20);
	float a = 0.0f, b = -5.0f, c = 11.8f;
	return x * x * x * x + y * y * y * y + z * z * z * z + a * (x * x + y * y + z * z) * (x * x + y * y + z * z) +
		b * (x * x + y * y + z * z) + c;
}

float f1(const vec3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	return x * x + y * y + z * z - 1;
}

float f2(const vec3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	return x * x/4 + y * y/16 + z * z/4 - 1;
}

void FunctionVisualizerScene::update(float dt)
{

	Scene::update(dt);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	vector<const char*> function_names
	{
		"x^4+y^4+z^4-5(x^2+y^2+z^2)+11.8=0",
		"x^2+y^2+z^2-1=0",
		"x^2/4+y^2/16+z^2/4-1=0"
	};
	vector<SDF> functions
	{
		f0,
		f1,
		f2,
	};
	static int prev_selected_function_index = -1;
	static int curr_selected_function_index = 0;

	if (ImGui::Begin("Functions"))
	{
		ImGui::Combo("Scenes", &curr_selected_function_index, &function_names[0], function_names.size());

		if (curr_selected_function_index != prev_selected_function_index)
		{
			for (int i = 0; i < objects.size(); i++)
			{
				delete objects[i];
			}
			auto p0 = vec3(-5.5f);
			auto p1 = vec3(5.5);
			BoundBox b(p0, p1);
			objects.clear();
			implicit_function_drawer = new ImplicitFunctionDrawer(functions[curr_selected_function_index], vec3(0.05), b, true);
			implicit_function_drawer->setShader(function_shader);
			//implicit_function_drawer->setNormalShader(normals_shader);
			objects.push_back(implicit_function_drawer);

		}
		prev_selected_function_index = curr_selected_function_index;
		//scenes[selected_scene_index]->update(dt);
		//scenes[selected_scene_index]->render();
		ImGui::End();
	}

}


vec3 torus(float u, float v)
{
	float R = 1;
	float r = 0.5;

	return vec3((R + r * cos(u)) * cos(v), (R + r * cos(u)) * sin(v), r * sin(u));
}


FunctionVisualizerScene::FunctionVisualizerScene(CameraBase* camera) : Scene(camera)
{
	auto p0 = vec3(-5.5f);
	auto p1 = vec3(5.5);
	bb_shader = new DefaultShader(camera, ROOT_DIR "src/shaders/bound_box.vert", ROOT_DIR "src/shaders/bound_box.frag");
	BoundBox b(p0, p1);


	function_shader = new AmbientDiffuseSpecularShader(camera);
	//implicit_function_drawer = new ImplicitFunctionDrawer(f, vec3(0.05), b, true);

	////flat_shader = new FlatADSShader(camera);
	//implicit_function_drawer->setShader(function_shader);

	////BoundBox a(vec3(-1), vec3(1));


	//float pi = glm::pi<float>();
	//parametric_function_drawer = new ParametricFunctionDrawer(torus, vec2(0.0f, 2 * pi), vec2(0, pi * 2),
	//	ivec2(80, 80));
	//parametric_function_drawer->setShader(function_shader);

	//Plane* plane = new Plane(5, 5);
	//plane->setShader(function_shader);
	////bb = new BoundBoxRenderer(a);
	////bb->setShader(bb_shader);

	normals_shader = new NormalsShader(camera);
	//implicit_function_drawer->setNormalShader(normals_shader);

	////add(implicit_function_drawer);
	////add(parametric_function_drawer);
	////add(plane);


}
