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

std::vector<vector<Manipulator*>> mans;

float lerp(float a, float b, float t)
{
	return a * (1 - t) + b * t;
}
glm::vec3 lerp(const glm::vec3& a, const glm::vec3& b, float t)
{
	return a * (1 - t) + b * t;
}
Line* line;
Line* line2;
Line* line3;
Sphere* sphere;


vec3 l(std::vector<pair<vec3, vec3>>& lines, float t = 0, int i = 0)
{
	if (i == lines.size() - 1)
	{
		vec3 v = lerp(lines[i].first, lines[i].second, t);
		return v;
	}
	vec3 v = lerp(lines[i].first, lines[i].second, t);
	return lerp(v, l(lines, t, i + 1), t);
}
vector<Line*> lines;

void FunctionVisualizerScene::update(float dt)
{
	Scene::update(dt);
	//static float u = 0;
	//static float v = 0;
	//
	//ImGui::DragFloat("u", &u, 0.01, 0, 1);
	//ImGui::DragFloat("v", &v, 0.01, 0, 1);


	

	//vec3 p = lerp(
	//	lerp(
	//		lerp(p00, p01, t), 
	//		lerp(p10, p11, t),
	//		t),
	//	lerp(
	//		lerp(p10, p11, t),
	//		lerp(p20, p21, t),
	//		t),
	//	t);
	//vec3 p = l(lines, t);
	//vec3 p = lerp(lerp(p00, p01, t), lerp(p10, p11, t), t);
	//sphere->setWorldPosition(p);
	//for (int i = 0; i < mans.size(); i++)
	//{
	//	vec3 pos = mans[i]->getWorldPosition();
	//	float* v = (float*)&pos;
	//	char buff[256];
	//	sprintf(buff, "Position %d %d\n", i% mans.size(), i/ mans.size());
	//	ImGui::DragFloat3(buff, v, 0.2, -10, 10);
	//	mans[i]->setWorldPosition(pos);
	//}
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

float powFast(float v, int n)
{
	float s = 1;
	for (int i = 0; i < n; i++)
	{
		s = s * v;
	}
	return s;
}

float stirling(int n)
{
	if (n <= 1)
		return 1.0f;
	float s = (float)n;
	float up = sqrt(2 * glm::pi<float>() * s);
	return floor(sqrt(2 * glm::pi<float>() * s)* powFast(s / glm::e<float>(), n)*(1 + 1.0/(12*s)));
}

int C(int n, int k)
{
	float s1 = stirling(n);
	float s2 = stirling(k);
	float s3 = stirling(n - k);
	return s1 / (s2 * s3);
}
float Bernshtein(int n, int k, float t)
{
	return C(n, k) * pow(t, k) * pow(1 - t, n - k);
}
vec3 Bezier(float t, vector<vec3> points)
{
	int n = points.size() - 1;
	vec3 result = Bernshtein(n, 0, t) * points[0];
	for (int k = 1; k <= n; k++)
	{
		result += Bernshtein(n, k, t) * points[k];
	}

	return result;
}

vec3 ff(const vec2 uv)
{
	float u = uv.x;
	float v = uv.y;
	int ss = mans.size();
	int i = u * (ss - 1);
	int j = v * (ss - 1);

	//vector<vec3> sss;
	//for (size_t k = 0; k < ss; k++)
	//{
	//	for (size_t j = 0; j < ss; j++)
	//	{
	//		sss.push_back(mans[k][j]->getWorldPosition());
	//	}
	//}

	auto n =  mans.size()-1;
	auto m =  mans.size()-1;
	auto result = vec3(0);
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= m; j++)
		{
			result += mans[i][j]->getWorldPosition() * Bernshtein(n, i, u) * Bernshtein(m, j, v);
		}
	}
	return result;

	//std::vector<pair<vec3, vec3>> vertical;
	//
	//for (size_t k = 0; k < ss - 1; k++)
	//{
	//	vertical.push_back(make_pair(mans[k][j]->getWorldPosition(), mans[k + 1][j]->getWorldPosition()));
	//}
	//
	//std::vector<pair<vec3, vec3>> horizontal;
	//for (size_t k = 0; k < ss - 1; k++)
	//{
	//	horizontal.push_back(make_pair(mans[i][k]->getWorldPosition(), mans[i][k+1]->getWorldPosition()));
	//}
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
	line = new Line(glm::vec3(0), glm::vec3(0, 0, 1));
	line2 = new Line(glm::vec3(0, 0, 1), glm::vec3(0, 1, 2));
	line3 = new Line(glm::vec3(0, 1, 2), glm::vec3(0, 4, -2));
	sphere = new Sphere(0.05);
	add(sphere);
	sphere->setShader(function_shader);
	add(line);
	line->setShader(function_shader);
	add(line2);
	line2->setShader(function_shader);
	add(line3);
	line3->setShader(function_shader);
	//auto implicit_function_drawer = new ImplicitFunctionDrawer(config);
	//implicit_function_drawer->task->onProcessEvent.add(bind(&FunctionVisualizerScene::on_progress, this, placeholders::_1));
	//implicit_function_drawer->setShader(function_shader);
	int n = 8;
	for (int i = 0; i < 8; i++)
	{
		vector<Manipulator*> m;
		for (int j = 0; j < 8; j++)
		{
			auto man = new Manipulator(camera);
			man->setWorldPosition(vec3(i - n/2, sin(i + j), j - n/2));
			m.push_back(man);
			add(man);
		}
		mans.push_back(m);
	}
	float pi = glm::pi<float>();
	auto parametric_function_drawer = new ParametricFunctionDrawer(*new ParametricFunctionConfiguration{
		"u",
		"v",
		 "u^2 - v^2",
		 ff,
		 vec2(0, 1),
		 vec2(0, 1)
		});


	//auto parametric_function_drawer = new ParametricFunctionDrawer(bind(&ParametricFunction::calculate, parametric, placeholders::_1), vec2(0.0f, 2 * pi), vec2(0, pi * 2), ivec2(80, 80), false, true);
	parametric_function_drawer->task->onProcessEvent.add(bind(&FunctionVisualizerScene::on_progress, this, placeholders::_1));
	parametric_function_drawer->setShader(function_shader);
	add(parametric_function_drawer);
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

}
