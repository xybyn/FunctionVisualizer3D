#ifndef OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H
#define OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H

#include <thread>
#include "common/WorldObject.h"
#include "common/trees/CubeTree.h"
#include "common/abstracts/BoundBox.h"
#include "common/tasks/ParallelTask.h"

typedef std::function<glm::vec3(const glm::vec2&)> DoubleVarFunction;

struct ParametricFunctionConfiguration
{
	const char* x_func;
	const char* y_func;
	const char* z_func;
	DoubleVarFunction f;
	glm::vec2 u_range;
	glm::vec2 v_range;
	glm::ivec2 divisions = glm::ivec2(80, 80);
	bool inverted = false;
	bool postpone = false;
};

class ParametricFunctionDrawer : public WorldObject
{
public:
	CubeTree<int>* tree;
	BoundBox* bb = nullptr;

	void update(float dt) override;
	void build() override;

	void render() override;

public:
	ParametricFunctionDrawer(const ParametricFunctionConfiguration& config);


public:
	ParametricFunctionConfiguration config;
	class ParametricFunctionDrawerTask : public ParallelTask<int>
	{
	public:
		ParametricFunctionDrawerTask(ParametricFunctionDrawer* owner);

		void run() override;

		~ParametricFunctionDrawerTask()
		{
			t->join();
			delete t;
		}

	private:
		std::thread* t = nullptr;

		void process(ParametricFunctionDrawerTask* owner);

		ParametricFunctionDrawer* owner = nullptr;
	} *task = nullptr;
	friend  class ParametricFunctionDrawerTask;
};

#endif //OPENGLPROJECT_PARAMETRICFUNCTIONDRAWER_H
