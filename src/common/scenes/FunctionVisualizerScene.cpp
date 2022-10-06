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

float progress = 0.0f;

void on_progress_changed(float p)
{
	progress = p;
}

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
float pow2(float v)
{
	return v * v;
}
float f3(const vec3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	typedef struct Metaball
	{
		vec3 position;
		float radius;
	};
	vector<Metaball> metaballs
	{
		Metaball {vec3(0), 0.65},
		Metaball {vec3(1), 0.25},
		Metaball {vec3(-1.5, 0.5, 0.5), 0.35},
	};
	float sum = 0;
	for (int i = 0; i < metaballs.size(); i++)
	{
		const Metaball& metaball = metaballs[i];
		sum += metaball.radius / sqrt(pow2(x - metaball.position.x) + pow2(y - metaball.position.y) + pow2(z - metaball.position.z));
	}
	return sum-1;
}

float f4(const vec3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	float a = 0.4f;
	float r = 1.2f;
	return pow2(x * x + y * y + z * z + r * r - a * a) - 4 * r * r * (x * x + y * y);
}
float f5(const vec3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;

	return 2*y*(y*y - 3*x*x)*(1-z*z) + pow2(x*x + y*y) - (9*z*z - 1) * (1-z*z);
}
float f6(const vec3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;
	float a = 0.2f;
	float R = 1.0f;
	float r = 0.1f;
	float f1 = pow2(x * x + y * y + z * z + R * R - a * a) - 4 * R * R * (x * x + y * y);
	float f2 = pow2(x * x + y * y + z * z + R * R - a * a) - 4 * R * R * (x * x + z * z);
	float f3 = pow2(x * x + y * y + z * z + R * R - a * a) - 4 * R * R * (z * z + y * y);
	return f1 * f2 * f3 - r;
}
void FunctionVisualizerScene::update(float dt)
{

	Scene::update(dt);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	vector<const char*> function_names
	{
		"x^4+y^4+z^4-5(x^2+y^2+z^2)+11.8=0",
		"x^2+y^2+z^2-1=0",
		"x^2/4+y^2/16+z^2/4-1=0",
		"metaballs",
		"torus",
		"genus",
		"three tori",
	};
	vector<SDF> functions
	{
		f0,
		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
	};
	static int prev_selected_function_index = -1;
	static int curr_selected_function_index = 0;

	if (ImGui::Begin("Functions"))
	{
		ImGui::Combo("Implicit Functions", &curr_selected_function_index, &function_names[0], function_names.size());

		if (curr_selected_function_index != prev_selected_function_index)
		{
			for (int i = 0; i < objects.size(); i++)
			{
				delete objects[i];
			}
			auto p0 = vec3(-2.5f);
			auto p1 = vec3(2.5);
			BoundBox b(p0, p1);
			objects.clear();
			implicit_function_drawer = new ImplicitFunctionDrawer(functions[curr_selected_function_index], vec3(0.025), b);
			implicit_function_drawer->setShader(function_shader);
			implicit_function_drawer->task->onProcessEvent.add(on_progress_changed);
			//implicit_function_drawer->setNormalShader(normals_shader);
			objects.push_back(implicit_function_drawer);

		}
		ImGui::ProgressBar(progress);
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
