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
#include "common/scenes/NURBSScene.h"
#include "common/tasks/HelloWorldTask.h"
#include "common/primitives/PointsCloud.h"
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
float powFast(float v, int n)
{
	float s = 1;
	for (int i = 0; i < n; i++)
	{
		s = s * v;
	}
	return s;
}
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
SingleColorShader* red_shader;
SingleColorShader* green_shader;
SingleColorShader* blue_shader;
SingleColorShader* white_shader;
std::vector<glm::vec4> controls_points;
float b0(float t)
{
	return powFast(1 - t, 3) / 6;
}
float b1(float t)
{
	return (4 - 6 * t * t + 3 * t * t * t) / 6;
}
float b2(float t)
{
	return (1 + 3 * t + 3 * t * t - 3 * t * t * t) / 6;
}
float b3(float t)
{
	return (t * t * t) / 6;
}
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
std::vector<pair<vec3, vec3>> pair_points;
int d = 4;
void drawLines(Scene* scene, const vector<vec4>& points, Shader* shader, vector<WorldObject*>& wo)
{
	for (size_t i = 0; i < points.size() - 1; i++)
	{
		//auto l = new Sphere(0.01f);
		auto l = new Line(points[i], points[i + 1]);
		//l->setWorldPosition(points[i]);
		l->setShader(shader);
		scene->add(l);
		wo.push_back(l);
	}
}

void removeLines(Scene* scene, vector<WorldObject*>& wo)
{
	for (size_t i = 0; i < wo.size(); i++)
	{
		scene->remove(wo[i]);
		delete wo[i];
	}
}
vec3 local(vec3& p0, vec3& p1, vec3& p2, vec3& p3, float t)
{
	return p0 * b0(t) + p1 * b1(t) + p2 * b2(t) + p3 * b3(t);
}

float N(float u, int i, const vector<float>& ts)
{
	if (ts[i + 1] <= u && u < ts[i + 2])
		return b0(u - ts[i + 1]);
	if (ts[i] <= u && u < ts[i + 1])
		return b1(u - ts[i]);
	if (ts[i - 1] <= u && u < ts[i])
		return b2(u - ts[i - 1]);
	if (ts[i - 2] <= u && u < ts[i - 1])
		return b3(u - ts[i - 2]);
	return 0;
}
//vector<float> tss{ 0.000, 0.001, 0.002, 0.003, 1.000,  1.001,  1.002,  1.003 };
//vector<float> tss{ 0.000,0.000, 0.000, 0.000, 1.000,  1.00,  1.00,  1.00 };
//vector<float> tss{ 0/7.0f, 1 / 7.0f, 2 / 7.0f, 3 / 7.0f, 4 / 7.0f, 5 / 7.0f, 6 / 7.0f, 7 / 7.0f };
//vector<float> tss{ 0, 0,  0 , 0, 1, 2, 3, 4, 5, 5, 5, 5 };
// 
vector<float> tss;
float ts(int i)
{
	return tss[i];
}

vec4 ps(int i)
{
	//cout << "ps " << i << endl;

	int n = controls_points.size();

	return controls_points[i];
}
float ws(int i)
{
	//cout << "ps " << i << endl;

	int n = controls_points.size();
	return controls_points[i].w;
}


vec4 ps(int i, int j)
{
	//cout << "ps " << i << endl;

	int n = sqrt(controls_points.size());

	return controls_points[i * n + j];
}
float ws(int i, int j)
{
	//cout << "ps " << i << endl;

	int n = sqrt(controls_points.size());
	return controls_points[i * n + j].w;
}
float N(float u, int i, int k)
{
	if (k == 0)
	{
		if (ts(i - 2) <= u && u < ts(i - 1))
			return 1;
		return 0;
	}
	float a0 = (u - ts(i));
	float a1 = N(u, i, k - 1);
	float a2 = (ts(i + k - 2) - ts(i - 2));

	float b0 = (ts(i + k - 1) - u);
	float b1 = N(u, i + 1, k - 1);
	float b2 = (ts(i + k - 1) - ts(i - 1));
	float res = a0 * a1 / a2 + b0 * b1 / b2;
	return res;
}
float bb(float t, int i, int j, const vector<float>& ts)
{
	if (j == 0)
	{
		if (ts[i] <= t && t < ts[i + 1])
			return 1;
		return 0;
	}

	return (t - ts[i]) / (ts[i + j] - ts[i]) * bb(t, i, j - 1, ts) + (ts[i + j + 1] - t) / (ts[i + j + 1] - ts[i + 1]) * bb(t, i + 1, j - 1, ts);
}


float B(float x, int i, int p)
{
	if (p == 0)
	{
		if (ts(i) <= x && x < ts(i + 1))
		{
			return 1;
		}
		return 0;
	}

	float a0 = (x - ts(i));
	float b0 = B(x, i, p - 1);

	float b1 = (ts(i + p + 1) - x);

	float d0 = (ts(i + p) - ts(i));
	float d1 = (ts(i + p + 1) - ts(i + 1));
	if (d0 == 0)
	{
		a0 = 0;
		d0 = 1;
	}
	if (d1 == 0)
	{
		b1 = 0;
		d1 = 1;
	}
	return a0 / d0 * b0 + b1 / d1 * B(x, i + 1, p - 1);
}


float R(float u, int i)
{
	return B(u, i, 3);
}


glm::vec3 ff(const vec2& uv)
{
	float left = tss[3];
	float right = tss[tss.size() - 1 - 3];

	float u = left + uv.x * (right - left);
	float v = left + uv.y * (right - left);
	int n = sqrt(controls_points.size()) - 1;
	vec4 sum0 = vec4(0);
	float denom = 0.0;

	int i_curve = 0;
	int j_curve = 0;

	for (int i = 3; i < tss.size() - 1 - 3; i++)
	{
		if (tss[i] <= u && u < tss[i + 1])
			i_curve = i - 3;
	}

	for (int i = 3; i < tss.size() - 1 - 3; i++)
	{
		if (tss[i] <= v && v < tss[i + 1])
			j_curve = i - 3;
	}

	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j <= 3; j++)
		{
			sum0 += ws(i_curve + i, j_curve + j) *
				ps(i_curve + i, j_curve + j) *
				B(u, i_curve + i, 3) *
				B(v, j_curve + j, 3);
		}
	}

	for (int i = 0; i <= 3; i++)
	{
		for (int j = 0; j <= 3; j++)
		{
			denom += ws(i_curve + i, j_curve + j) *
				B(u, i_curve + i, 3) *
				B(v, j_curve + j, 3);
		}
	}

	return sum0 / denom;
}


void FunctionVisualizerScene::update(float dt)
{
	Scene::update(dt);

	int n = sqrt(controls_points.size());
	if (ImGui::CollapsingHeader("Points"))
	{

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				char buffer[128];
				sprintf(buffer, "p %d %d", i, j);
				ImGui::SliderFloat3(buffer, (float*)&controls_points[i * n + j], -10, 10);

			}
		}
	}

	if (ImGui::CollapsingHeader("Weights"))
	{

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				char buffer[128];
				sprintf(buffer, "w %d %d", i, j);
				ImGui::SliderFloat(buffer, &controls_points[i * n + j].w, -10, 10);

			}
		}
	}


	if (ImGui::CollapsingHeader("Knots"))
	{

		for (int j = 0; j < tss.size(); j++)
		{
			char buffer[128];
			sprintf(buffer, "t %d", j);
			ImGui::SliderFloat(buffer, &tss[j], -10, 10);

		}
	}

	if (ImGui::Button("Rebuild"))
	{

		ParametricFunctionConfiguration config
		{
			"",
			"",
			"",
			ff,
			vec2(0, 1),
			vec2(0, 1),
			ivec2(50, 50),
		};
		objects.clear();
		int n = sqrt(controls_points.size());
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				auto pos = controls_points[n * i + j];
				auto s = new Sphere(0.04f);
				s->setWorldPosition(pos);
				s->setShader(red_shader);
				add(s);
			}
		}


		remove(current_parametric_function_drawer);
		delete current_parametric_function_drawer;
		current_parametric_function_drawer = new ParametricFunctionDrawer(config);
		current_parametric_function_drawer->setShader(function_shader);
		add(current_parametric_function_drawer);



		{
			int num = 80;

			int n = sqrt(controls_points.size()) - 1;
			float left = tss[3];
			float right = tss[tss.size() - 1 - 3];
			float u_step = (right - left) / (float)num;
			float u = left;
			vector<vec4> pts;
			for (int u_counter = 0; u_counter <= num; u_counter++)
			{
				float v = left;
				for (int v_counter = 0; v_counter <= num; v_counter++)
				{
					vec4 sum0 = vec4(0);
					float denom = 0.0;

					int i_curve = 0;
					int j_curve = 0;

					for (int i = 3; i < tss.size() - 1 - 3; i++)
					{
						if (tss[i] <= u && u < tss[i + 1])
							i_curve = i - 3;
					}

					for (int i = 3; i < tss.size() - 1 - 3; i++)
					{
						if (tss[i] <= v && v < tss[i + 1])
							j_curve = i - 3;
					}

					for (int i = 0; i <= 3; i++)
					{
						for (int j = 0; j <= 3; j++)
						{
							sum0 += ws(i_curve + i, j_curve + j) *
								ps(i_curve + i, j_curve + j) *
								B(u, i_curve + i, 3) *
								B(v, j_curve + j, 3);
						}
					}

					for (int i = 0; i <= 3; i++)
					{
						for (int j = 0; j <= 3; j++)
						{
							denom += ws(i_curve + i, j_curve + j) *
								B(u, i_curve + i, 3) *
								B(v, j_curve + j, 3);
						}
					}

					pts.push_back((sum0 / denom));
					v += u_step;
				}
				u += u_step;
			}

			static PointsCloud* cloud = nullptr;
			if (cloud)
			{
				remove(cloud);
				delete cloud;
			}
			cloud = new PointsCloud(pts);
			cloud->setShader(green_shader);
			add(cloud);

			int nn = sqrt(controls_points.size());

			for (int i = 0; i < nn; i++)
			{
				vector<vec4> slice;
				for (int j = 0; j < nn; j++)
				{
					slice.push_back(controls_points[i * nn + j]);
				}

				vector<WorldObject*>wo;
				drawLines(this, slice, red_shader, wo);

			}

			for (int j = 0; j < nn; j++)
			{
				vector<vec4> slice;
				for (int i = 0; i < nn; i++)
				{
					slice.push_back(controls_points[i * nn + j]);
				}

				vector<WorldObject*>wo;
				drawLines(this, slice, red_shader, wo);

			}
		}

	}
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



float stirling(int n)
{
	if (n <= 1)
		return 1.0f;
	float s = (float)n;
	float up = sqrt(2 * glm::pi<float>() * s);
	return floor(sqrt(2 * glm::pi<float>() * s) * powFast(s / glm::e<float>(), n) * (1 + 1.0 / (12 * s)));
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


FunctionVisualizerScene::FunctionVisualizerScene(CameraBase* camera) : Scene(camera)
{

	auto p0 = vec3(-5.5f);
	auto p1 = vec3(5.5);
	// bb_shader = new DefaultShader(camera, ROOT_DIR "src/shaders/bound_box.vert", ROOT_DIR "src/shaders/bound_box.frag");
	BoundBox b(p0, p1);

	function_shader = new AmbientDiffuseSpecularShader(camera);
	float scal = 0.6;



	red_shader = new SingleColorShader(camera);
	red_shader->setColor(vec3(1, 0, 0));

	green_shader = new SingleColorShader(camera);
	green_shader->setColor(vec3(0, 1, 0));

	white_shader = new SingleColorShader(camera);
	white_shader->setColor(vec3(1, 1, 1));

	int n = 8;
	tss.push_back(0);
	tss.push_back(0);
	tss.push_back(0);
	tss.push_back(0);
	for (int i = 1; i <= n - 3; i++)
	{
		tss.push_back(i);
	}
	tss.push_back(tss[tss.size() - 1]);
	tss.push_back(tss[tss.size() - 1]);
	tss.push_back(tss[tss.size() - 1]);

	//for (int i = 0; i <= n + 3; i++)
	//{
	//	tss.push_back(i);
	//}

	//controls_points.push_back(vec4(-4, -4, 0, 1));
	//controls_points.push_back(vec4(-2, 4, 0, 1));
	//controls_points.push_back(vec4(2, -4, 0, 1));
	//controls_points.push_back(vec4(4, 4, 0, 1));
	//
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			vec4 pos;
			if (i == 0 || j == 0 || j == n - 1 || i == n - 1)
			{
				pos.y = 0;
			}
			else if (i == 1 || j == 1 || j == n - 2 || i == n - 2)
			{
				pos.y = -1;
			}
			else
			{
				pos.y = 2;
			}
			float x = i - n / 2;
			float z = j - n / 2;
			pos.x = x;
			pos.z = z;
			pos.w = 1;
			//vec4 pos;

			controls_points.push_back(pos);
		}
	}

	//
	//ParametricFunctionConfiguration config
	//{
	//	"",
	//	"",
	//	"",
	//	ff,
	//	vec2(0, 1),
	//	vec2(0, 1),
	//	ivec2(50, 50),
	//};
	//current_parametric_function_drawer = new ParametricFunctionDrawer(config);
	//current_parametric_function_drawer->setShader(function_shader);
	//add(current_parametric_function_drawer);

}
