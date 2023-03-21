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

std::vector<vector<vec3>> points;

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
void drawlines(Scene* scene, const vector<vec3>& v, Shader* shader)
{
	if (v.size() < 2)
		return;
	for (int i = 0; i < v.size() - 1; i++)
	{
		auto line = new Line(v[i], v[i + 1]);
		line->setShader(shader);
		scene->add(line);

	}
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
	return ceil(sqrt(2 * glm::pi<float>() * s) * powFast(s / glm::e<float>(), n) * (1 + 1.0 / (12 * s)));
}

int C(int n, int k)
{
	float s1 = stirling(n);
	float s2 = stirling(k);
	float s3 = stirling(n - k);
	return s1 / (s2 * s3);
}
vector<vector<int>> pas;
void prepare_pascal(int n, int k)
{
	for (int i = 0; i < n + 1; i++)
	{
		pas.push_back(vector<int>());
		pas[i].push_back(1);
		for (int j = 1; j < i; j++)
		{
			pas[i].push_back(pas[i - 1][j - 1] + pas[i - 1][j]);
		}
		if (i != 0)
		{
			pas[i].push_back(1);
		}
	}
}
int CPascal(int n, int k)
{
	
	return pas[n][k];
}
float Bernshtein(int n, int k, float t)
{
	float c = CPascal(n, k);
	return c * pow(t, k) * pow(1 - t, n - k);
}
vec3 ff(const vec2 uv)
{
	float u = uv.x;
	float v = uv.y;
	int ss = points.size();
	int i = u * (ss - 1);
	int j = v * (ss - 1);

	auto n = points.size() - 1;
	auto m = points.size() - 1;
	auto result = vec3(0);
	for (int i = 0; i <= n; i++)
	{
		for (int j = 0; j <= m; j++)
		{
			result += points[i][j] * Bernshtein(n, i, u) * Bernshtein(m, j, v);
		}
	}
	return result;
}
int degree = 5;
void FunctionVisualizerScene::update(float dt)
{
	Scene::update(dt);
	ImGui::ProgressBar(progress);
	ImGui::InputInt("Degree", &degree);


	for (int i = 0; i < points.size(); i++)
	{
		vector<vec3>& v = points[i];
		for (int j = 0; j < v.size(); j++)
		{
			char buffer[256];
			sprintf(buffer, "P %d %d", i, j);
			ImGui::DragFloat3(buffer, (float*)&v[j], 0.1, -5, 5);
		}
	}

	if (ImGui::Button("Update"))
	{
		removeAll();
		
		for (int i = 0; i <= degree; i++)
		{
			for (int j = 0; j <= degree; j++)
			{
				auto s = new Sphere(0.01);
				s->setWorldPosition(points[i][j]);
				s->setShader(red);
				add(s);
			}
		}
		for (int i = 0; i <= degree; i++)
		{
			vector<vec3> row;
			for (int j = 0; j <= degree; j++)
			{
				row.push_back(points[i][j]);
			}
			drawlines(this, row, red);
		}

		for (int i = 0; i <= degree; i++)
		{
			vector<vec3> row;
			for (int j = 0; j <= degree; j++)
			{
				row.push_back(points[j][i]);
			}
			drawlines(this, row, red);
		}

		float pi = glm::pi<float>();
		auto parametric_function_drawer = new ParametricFunctionDrawer(*new ParametricFunctionConfiguration{
			"",
			"",
			 "",
			 ff,
			 vec2(0, 1),
			 vec2(0, 1)
			});


		//auto parametric_function_drawer = new ParametricFunctionDrawer(bind(&ParametricFunction::calculate, parametric, placeholders::_1), vec2(0.0f, 2 * pi), vec2(0, pi * 2), ivec2(80, 80), false, true);
		parametric_function_drawer->task->onProcessEvent.add(bind(&FunctionVisualizerScene::on_progress, this, placeholders::_1));
		parametric_function_drawer->setShader(function_shader);
		add(parametric_function_drawer);
	}
	if (ImGui::Button("Rebuild"))
	{
		points.clear();
		removeAll();
		for (int i = 0; i <= degree; i++)
		{
			vector<vec3> row;
			for (int j = 0; j <= degree; j++)
			{
				auto p = vec3(i - degree / 2, 0, j - degree / 2);
				row.push_back(p);
				auto s = new Sphere(0.01);
				s->setWorldPosition(p);
				s->setShader(red);
				add(s);
			}
			points.push_back(row);
		}

		for (int i = 0; i <= degree; i++)
		{
			vector<vec3> row;
			for (int j = 0; j <= degree; j++)
			{
				row.push_back(points[i][j]);
			}
			drawlines(this, row, red);
		}

		for (int i = 0; i <= degree; i++)
		{
			vector<vec3> row;
			for (int j = 0; j <= degree; j++)
			{
				row.push_back(points[j][i]);
			}
			drawlines(this, row, red);
		}

		float pi = glm::pi<float>();
		auto parametric_function_drawer = new ParametricFunctionDrawer(*new ParametricFunctionConfiguration{
			"",
			"",
			 "",
			 ff,
			 vec2(0, 1),
			 vec2(0, 1)
			});


		//auto parametric_function_drawer = new ParametricFunctionDrawer(bind(&ParametricFunction::calculate, parametric, placeholders::_1), vec2(0.0f, 2 * pi), vec2(0, pi * 2), ivec2(80, 80), false, true);
		parametric_function_drawer->task->onProcessEvent.add(bind(&FunctionVisualizerScene::on_progress, this, placeholders::_1));
		parametric_function_drawer->setShader(function_shader);
		add(parametric_function_drawer);
	}

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




FunctionVisualizerScene::FunctionVisualizerScene(CameraBase* camera) : Scene(camera)
{

	auto p0 = vec3(-5.5f);
	auto p1 = vec3(5.5);
	// bb_shader = new DefaultShader(camera, ROOT_DIR "src/shaders/bound_box.vert", ROOT_DIR "src/shaders/bound_box.frag");
	BoundBox b(p0, p1);

	auto s = new ImplicitFunction("sqrt(x*x + y*y + z*z)-0.5");

	auto parametric = new ParametricFunction("(1 + 0.5 * cos(u)) * cos(v)", "(1 + 0.5 * cos(u)) * sin(v)", "0.5*sin(u)");

	function_shader = new AmbientDiffuseSpecularShader(camera);
	prepare_pascal(100, 100);
	red = new SingleColorShader(camera);
	red->setColor(vec3(1, 0, 0));
	
}
