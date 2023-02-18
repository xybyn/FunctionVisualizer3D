#include "FunctionVisualizerScene.h"
#include <vector>
#include "common/primitives/BoundBoxRenderer.h"
#include "common/trees/CubeTree.h"
#include "common/trees/CubeTreeRenderer.h"
#include <common/primitives/Sphere.h>
#include "common/Texture.h"
#include "common/primitives/Point.h"
#include "common/shaders/NormalsShader.h"
#include "common/shaders/SingleColorShader.h"
#include "common/primitives/Plane.h"
#include "common/primitives/Line.h"
#include "common/primitives/Manipulator.h"

#include "common/tasks/HelloWorldTask.h"
#include "exprtk/exprtk.hpp"
using namespace std;
using namespace glm;
float progress = 0.0f;

enum class FUNCTION_TYPE
{
	IMPLICIT,
	PARAMETRIC,
	EXPLICIT
};
class Function
{
public:
	typedef exprtk::symbol_table<float> symbol_table_t;
	typedef exprtk::expression<float>   expression_t;
	typedef exprtk::parser<float>       parser_t;
	expression_t expression;
	symbol_table_t symbol_table;
	parser_t parser;

};
class TwoVariableFunction : public Function
{
public:
	float u;
	float v;
	TwoVariableFunction(const std::string& s)
	{
		symbol_table.add_variable("u", u);
		symbol_table.add_variable("v", v);
		symbol_table.add_constants();

		expression.register_symbol_table(symbol_table);

		parser.compile(s, expression);
	}

	float calculate(const vec2& value)
	{
		this->u = value.x;
		this->v = value.y;
		return expression.value();
	}
};
class ImplicitFunction : public Function
{
public:
	float x;
	float y;
	float z;
	ImplicitFunction(const std::string& s)
	{

		symbol_table.add_variable("x", x);
		symbol_table.add_variable("y", y);
		symbol_table.add_variable("z", z);
		symbol_table.add_constants();

		expression.register_symbol_table(symbol_table);

		parser.compile(s, expression);
	}
	float calculate(const vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return expression.value();
	}
};



class ParametricFunction : public Function
{
public:
	TwoVariableFunction* x_function;
	TwoVariableFunction* y_function;
	TwoVariableFunction* z_function;
	ParametricFunction(const std::string& x_func,
		const std::string& y_func,
		const std::string& z_func
	)
	{
		x_function = new TwoVariableFunction(x_func);
		y_function = new TwoVariableFunction(y_func);
		z_function = new TwoVariableFunction(z_func);
	}
	vec3 calculate(const vec2& p)
	{
		return vec3(x_function->calculate(p),
			y_function->calculate(p),
			z_function->calculate(p));
	}
};
map<const char*, ImplicitFunctionConfiguration*> implicit_functions_map;
map<const char*, ParametricFunctionConfiguration*> parametric_functions_map;

map<const char*, FUNCTION_TYPE>types_map;

ImplicitFunctionConfiguration* cuurent_implicit_config = nullptr;
ParametricFunctionConfiguration* cuurent_parametric_config = nullptr;
class ImplicitFunction* cuurent_implicit_function = nullptr;
class ParametricFunction* cuurent_parametric_function = nullptr;
ImplicitFunctionDrawer* current_implicit_function_drawer = nullptr;
ParametricFunctionDrawer* current_parametric_function_drawer = nullptr;

std::vector<Manipulator*> mans;

void FunctionVisualizerScene::update(float dt)
{
	Scene::update(dt);
	for (int i = 0; i < mans.size(); i++)
	{
		vec3 pos = mans[i]->getWorldPosition();
		float* v = (float*)&pos;
		char buff[256];
		sprintf(buff, "Position %d %d\n", i% mans.size(), i/ mans.size());
		ImGui::DragFloat3(buff, v, 0.2, -10, 10);
		mans[i]->setWorldPosition(pos);
	}
	//man->setWorldPosition(glm::vec3(v[0], v[1], v[2]));

	//static bool plane_added = true;
	//
	//if (ImGui::BeginTabBar(""))
	//{
	//	if (ImGui::BeginTabItem("Implicit"))
	//	{
	//		static int selected_implicit = 0;
	//		static int selected_implicit_prev = ~selected_implicit;
	//		vector<const char*> function_names;
	//		for (auto& it = implicit_functions_map.begin(); it != implicit_functions_map.end(); it++)
	//		{
	//			function_names.push_back(it->first);
	//		}
	//		ImGui::Combo("Examples", &selected_implicit, &function_names[0], function_names.size());
	//
	//		static char buffer[256];
	//
	//		static float min_point[3];
	//		static float max_point[3];
	//		static float density[3];
	//
	//		if (selected_implicit != selected_implicit_prev)
	//		{
	//			ImplicitFunctionConfiguration c = *implicit_functions_map.find(function_names[selected_implicit])->second;
	//			memcpy(&buffer[0], c.description, strlen(c.description) + 1);
	//			min_point[0] = c.volume.getLeftBottomBack().x;
	//			min_point[1] = c.volume.getLeftBottomBack().y;
	//			min_point[2] = c.volume.getLeftBottomBack().z;
	//
	//			max_point[0] = c.volume.getRightUpperFront().x;
	//			max_point[1] = c.volume.getRightUpperFront().y;
	//			max_point[2] = c.volume.getRightUpperFront().z;
	//
	//			density[0] = 1 / c.steps.x;
	//			density[1] = 1 / c.steps.y;
	//			density[2] = 1 / c.steps.z;
	//		}
	//		selected_implicit_prev = selected_implicit;
	//		ImGui::Text("F(x, y, z) =");
	//		ImGui::SameLine();
	//		ImGui::InputText("= 0", buffer, 256);
	//		ImGui::Text("Volume");
	//		ImGui::DragFloat3("Min point", min_point, 0.1, -10, 10);
	//		ImGui::DragFloat3("Max point", max_point, 0.1, -10, 10);
	//
	//		ImGui::Text("Density on unit");
	//		ImGui::DragFloat3("Density", density, 0.1, 0.1, 100);
	//
	//		BoundBox bb(vec3(min_point[0], min_point[1], min_point[2]), vec3(max_point[0], max_point[1], max_point[2]));
	//		vec3 steps = vec3(1 / density[0], 1 / density[1], 1 / density[2]);
	//
	//
	//		if (ImGui::Button("Build"))
	//		{
	//			delete cuurent_implicit_config;
	//			delete cuurent_implicit_function;
	//			remove(current_implicit_function_drawer);
	//			remove(current_parametric_function_drawer);
	//			delete current_parametric_function_drawer;
	//			delete current_implicit_function_drawer;
	//			auto m = string(buffer);
	//
	//			cuurent_implicit_function = new ImplicitFunction(m);
	//			cuurent_implicit_config = new ImplicitFunctionConfiguration
	//			{
	//				"",
	//				bind(&ImplicitFunction::calculate, cuurent_implicit_function, placeholders::_1),
	//				steps,
	//				bb,
	//				true,
	//				false
	//			};
	//
	//
	//			current_implicit_function_drawer = new ImplicitFunctionDrawer(*cuurent_implicit_config);
	//			current_implicit_function_drawer->task->onProcessEvent.add(bind(&FunctionVisualizerScene::on_progress, this, placeholders::_1));
	//			current_implicit_function_drawer->setShader(function_shader);
	//			add(current_implicit_function_drawer);
	//		}
	//		ImGui::EndTabItem();
	//	}
	//	if (ImGui::BeginTabItem("Parametric"))
	//	{
	//		static int selected_parametric = 0;
	//		static int selected_parametric_prev = ~selected_parametric;
	//		vector<const char*> function_names;
	//
	//		static char x_buffer[256];
	//		static char y_buffer[256];
	//		static char z_buffer[256];
	//		static float u_min_max[2];
	//		static float v_min_max[2];
	//		static int divisions[2];
	//		for (auto& it = parametric_functions_map.begin(); it != parametric_functions_map.end(); it++)
	//		{
	//			function_names.push_back(it->first);
	//		}
	//		ImGui::Combo("Examples", &selected_parametric, &function_names[0], function_names.size());
	//
	//		if (selected_parametric != selected_parametric_prev)
	//		{
	//			ParametricFunctionConfiguration c = *parametric_functions_map.find(function_names[selected_parametric])->second;
	//			memcpy(&x_buffer[0], c.x_func, strlen(c.x_func) + 1);
	//			memcpy(&y_buffer[0], c.y_func, strlen(c.y_func) + 1);
	//			memcpy(&z_buffer[0], c.z_func, strlen(c.z_func) + 1);
	//
	//			u_min_max[0] = c.u_range.x;
	//			u_min_max[1] = c.u_range.y;
	//
	//			v_min_max[0] = c.v_range.x;
	//			v_min_max[1] = c.v_range.y;
	//
	//			divisions[0] = c.divisions.x;
	//			divisions[1] = c.divisions.y;
	//		}
	//		selected_parametric_prev = selected_parametric;
	//
	//
	//
	//		ImGui::InputText("= x(u, v)", &x_buffer[0], 256);
	//
	//		ImGui::InputText("= y(u, v)", &y_buffer[0], 256);
	//
	//		ImGui::InputText("= z(u, v)", &z_buffer[0], 256);
	//
	//
	//		ImGui::InputFloat2("u range", u_min_max);
	//		ImGui::InputFloat2("v range", v_min_max);
	//
	//
	//		ImGui::InputInt2("Divisions", divisions);
	//
	//		if (ImGui::Button("Build"))
	//		{
	//			delete cuurent_parametric_config;
	//			delete cuurent_parametric_function;
	//			remove(current_parametric_function_drawer);
	//			delete current_parametric_function_drawer;
	//			remove(current_implicit_function_drawer);
	//			delete current_implicit_function_drawer;
	//			vec2 u_range = vec2(u_min_max[0], u_min_max[1]);
	//			vec2 v_range = vec2(v_min_max[0], v_min_max[1]);
	//			ivec2 divisions_vec = ivec2(divisions[0], divisions[1]);
	//
	//			cuurent_parametric_function = new ParametricFunction(string(x_buffer), string(y_buffer), string(z_buffer));
	//			cuurent_parametric_config = new ParametricFunctionConfiguration
	//			{
	//				"",
	//				"",
	//				"",
	//				bind(&ParametricFunction::calculate, cuurent_parametric_function, placeholders::_1),
	//				u_range,
	//				v_range,
	//				divisions_vec
	//			};
	//
	//
	//			current_parametric_function_drawer = new ParametricFunctionDrawer(*cuurent_parametric_config);
	//			current_parametric_function_drawer->task->onProcessEvent.add(bind(&FunctionVisualizerScene::on_progress, this, placeholders::_1));
	//			current_parametric_function_drawer->setShader(function_shader);
	//			add(current_parametric_function_drawer);
	//		}
	//
	//		ImGui::EndTabItem();
	//	}
	//	ImGui::EndTabBar();
	//}
	//ImGui::Text("Generating progress...");
	//ImGui::ProgressBar(progress);
}

void FunctionVisualizerScene::on_progress(float p)
{
	progress = p;
}


float FunctionVisualizerScene::f(const vec3& v)
{
	float x = v.x;
	float y = v.y;
	float z = v.z;

	//return x;
	float a = 0.0f, b = -5.0f, c = 11.8f;
	return x * x * x * x + y * y * y * y + z * z * z * z + a * (x * x + y * y + z * z) * (x * x + y * y + z * z) +
		b * (x * x + y * y + z * z) + c;
}

vec3 torus(const vec2& val)
{
	float u = val.x;
	float v = val.y;
	float R = 1;
	float r = 0.5;

	return vec3((R + r * cos(u)) * cos(v), (R + r * cos(u)) * sin(v), r * sin(u));
}



FunctionVisualizerScene::FunctionVisualizerScene(CameraBase* camera) : Scene(camera)
{
	auto p0 = vec3(-5.5f);
	auto p1 = vec3(5.5);
	// bb_shader = new DefaultShader(camera, ROOT_DIR "src/shaders/bound_box.vert", ROOT_DIR "src/shaders/bound_box.frag");
	BoundBox b(p0, p1);

	auto s = new ImplicitFunction("sqrt(x*x + y*y + z*z)-0.5");

	auto parametric = new ParametricFunction("(1 + 0.5 * cos(u)) * cos(v)", "(1 + 0.5 * cos(u)) * sin(v)", "0.5*sin(u)");

	function_shader = new AmbientDiffuseSpecularShader(camera);

	

	//auto implicit_function_drawer = new ImplicitFunctionDrawer(config);
	//implicit_function_drawer->task->onProcessEvent.add(bind(&FunctionVisualizerScene::on_progress, this, placeholders::_1));
	//implicit_function_drawer->setShader(function_shader);

	float pi = glm::pi<float>();
	//auto parametric_function_drawer = new ParametricFunctionDrawer(bind(&ParametricFunction::calculate, parametric, placeholders::_1), vec2(0.0f, 2 * pi), vec2(0, pi * 2), ivec2(80, 80), false, true);
	//parametric_function_drawer->task->onProcessEvent.add(bind(&FunctionVisualizerScene::on_progress, this, placeholders::_1));
	//parametric_function_drawer->setShader(function_shader);
	//float a = 0.0f, b = -5.0f, c = 11.8f;
	//return x * x * x * x + y * y * y * y + z * z * z * z + a * (x * x + y * y + z * z) * (x * x + y * y + z * z) +
	//	b * (x * x + y * y + z * z) + c;

	parametric_functions_map.insert(make_pair("Torus", new ParametricFunctionConfiguration{
		"(1 + 0.5 * cos(u)) * cos(v)",
		"(1 + 0.5 * cos(u)) * sin(v)",
		 "0.5*sin(u)",
		 nullptr,
		 vec2(0.0f, 2 * pi),
		 vec2(0, pi * 2)
		}));

	parametric_functions_map.insert(make_pair("Mebius", new ParametricFunctionConfiguration{
		"(1 + v/2*cos(u/2))*cos(u)",
		"(1 + v/2*cos(u/2))*sin(u)",
		 "v/2*sin(u / 2)",
		 nullptr,
		 vec2(0.0f, 2 * pi),
		 vec2(-1, 1)
		}));

	parametric_functions_map.insert(make_pair("Sadle", new ParametricFunctionConfiguration{
		"u",
		"v",
		 "u^2 - v^2",
		 nullptr,
		 vec2(-2, 2),
		 vec2(-2, 2)
		}));

	parametric_functions_map.insert(make_pair("Trefoil knot", new ParametricFunctionConfiguration{
		"u",
		"v",
		 "u^2 - v^2",
		 nullptr,
		 vec2(-2, 2),
		 vec2(-2, 2)
		}));

	//implicit_functions_map.insert(make_pair("Goursat Tangle", new ImplicitFunctionConfiguration{
	//	}));
	implicit_functions_map.insert(make_pair("Ellipsoid", new ImplicitFunctionConfiguration{
		"x^2/4 + y^2/1 + z^2/1 - 1",
		}));

	implicit_functions_map.insert(make_pair("Sphere", new ImplicitFunctionConfiguration{
		"x^2 + y^2 + z^2 - 1",
		}));
	implicit_functions_map.insert(make_pair("Plane", new ImplicitFunctionConfiguration{
		"x + y + z - 1",
		}));

	implicit_functions_map.insert(make_pair("Hyperboloid", new ImplicitFunctionConfiguration{
		"x^2/4 + y^2/1 - z^2/1 - 1",
		}));
	implicit_functions_map.insert(make_pair("Double Hyperboloid", new ImplicitFunctionConfiguration{
		"x^2/4 + y^2/1 - z^2/1 + 1",
		}));
	implicit_functions_map.insert(make_pair("Hyperbolyc Parabaloid", new ImplicitFunctionConfiguration{
		"z^2/4 - y^2/1 - 2*x",
		}));
	implicit_functions_map.insert(make_pair("Elliptic Parabaloid", new ImplicitFunctionConfiguration{
		"x^2/4 + z^2/16 - x/4 - 1",
		}));
	implicit_functions_map.insert(make_pair("Paraboloid Cylinder", new ImplicitFunctionConfiguration{
		"y^2 -2*3*x",
		}));
	implicit_functions_map.insert(make_pair("Hyperboloid Cylinder", new ImplicitFunctionConfiguration{
		"x^2 - y^2 - 1",
		}));

	implicit_functions_map.insert(make_pair("Torus", new ImplicitFunctionConfiguration{
		"(x^2 + y^2 + z^2 + 2^2 - 1^2)^2 - 4*2^2*(x^2 + y^2)",
		}));

	implicit_functions_map.insert(make_pair("Genus 2", new ImplicitFunctionConfiguration{
		"2y(y^2 -3x^2)(1-z^2) + (x^2 + y^2)^2 - (9z^2-1)(1-z^2)",
		}));
	implicit_functions_map.insert(make_pair("Wineglass", new ImplicitFunctionConfiguration{
		"x^2 + z^2 - (log(y+3.2))^2 - 0.02",
		}));

	//implicit_functions_map.insert(make_pair("Cassini Oval", new ImplicitFunctionConfiguration{
	//	"sqrt(((x-1)^2 + y^2 + z^2) * ((x+1)^2 + y^2 + z^2) * (x^2 + (y-1)^2 + z^2) * ((x^2 + (y+1)^2 + z^2)) - 1.1",
	//	bind(&ImplicitFunction::calculate, s, placeholders::_1),
	//	}));
	//functions_map.insert(make_pair("Torus", parametric_function_drawer));
	types_map.insert(make_pair("Implicit", FUNCTION_TYPE::IMPLICIT));
	types_map.insert(make_pair("Parametric", FUNCTION_TYPE::PARAMETRIC));

	plane = new Plane(5, 5);
	plane->setShader(function_shader);
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			auto man = new Manipulator(camera);
			man->setWorldPosition(vec3(i - 2, 0, j - 2));
			mans.push_back(man);
			add(man);

		}
	}
}
