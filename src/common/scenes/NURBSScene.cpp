//
// Created by pc on 02.08.22.
//

#include "NURBSScene.h"
#include "common/primitives/Sphere.h"
#include "common/function_drawers/ParametricFunctionDrawer.h"
using namespace glm;
using namespace std;

void NURBSScene::update(float dt) {
    Scene::update(dt);
    
}

namespace {
	std::vector<std::vector<vec4>> Ps;
}

NURBSScene::NURBSScene(CameraBase *camera) : Scene(camera) {
	ParametricFunctionConfiguration config
	{
		"",
		"",
		"",
		bind(&NURBSScene::func, this, placeholders::_1),
		vec2(0, 1),
		vec2(0, 1),
	};

	parametric_function = new ParametricFunctionDrawer(config);
	add(parametric_function);
	//controls_points.push_back(glm::vec4(0, 0, 0, 1 / scal) * scal);
	//controls_points.push_back(glm::vec4(0.5, 1, 0, 1 / scal) * scal);
	//controls_points.push_back(glm::vec4(1, -0.3, 0, 1 / scal) * scal);
	//controls_points.push_back(glm::vec4(2, -1, 0, 1 / scal) * scal);
	//controls_points.push_back(glm::vec4(3, -0.2, 0, 1 / scal) * scal);
	//controls_points.push_back(glm::vec4(2, 0.3, 0, 1 / scal) * scal);
	//controls_points.push_back(glm::vec4(2, 1.5, 0, 1 / scal) * scal);
	//controls_points.push_back(glm::vec4(3, 1.2, 0, 1 / scal) * scal);

	for (size_t i = 0; i < 8; i++)
	{
		vector<vec4> s;
		for (size_t j = 0; j < 8; j++)
		{
			s.push_back(vec4(i, j, 0, 1));
		}
		Ps.push_back(s);
	}
}

glm::vec3 NURBSScene::func(const glm::vec2& uv)
{
	return glm::vec3();
}


glm::vec3 NURBS::getPoint(float u)
{
	int n = pss.size() - 1;
	u = (n - 2)*u;



	int i = (int)u;

	vec4 numerator = vec4(0);
	for (int k = 0; k <= 3; k++)
	{
		numerator += ws(i + k) * ps(i + k) * B(u, i + k, 3);
	}
	float denominator = 0;
	for (int k = 0; k <= 3; k++)
	{
		denominator += B(u, i + k, 3) * ws(i + k);
	}

	return numerator / denominator;
}

float NURBS::B(float x, int i, int p)
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
