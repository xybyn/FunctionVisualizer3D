#ifndef OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H
#define OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H

#include "common/WorldObject.h"
#include "common/abstracts/BoundBox.h"
#include "common/tasks/ParallelTask.h"
#include "common/trees/CubeTree.h"
typedef float (*SDF)(const glm::vec3&);
class ImplicitFunctionChunk : public WorldObject
{
public:
	ImplicitFunctionChunk(SDF function, const glm::vec3& step, const BoundBox& bound, bool inverted = false);

	CubeTree<int>* tree = nullptr;
	SDF function;
	glm::vec3 step;
	BoundBox bound;
	bool inverted;

	friend class ImplicitFunctionChunkTask;
};

class ImplicitFunctionDrawer : public WorldObject
{
public:
	ImplicitFunctionDrawer(SDF function, const glm::vec3& step, const BoundBox& bound, bool inverted = false);
	class ImplicitFunctionChunkTask : public ParallelTask<void*>
	{
	public:
		ImplicitFunctionChunkTask(ImplicitFunctionChunk* chunk) : chunk(chunk) {}

		void process();
		void run() override;
		~ImplicitFunctionChunkTask() {
			t->join();
			delete t;
		}
	private:
		std::thread* t;
		ImplicitFunctionChunk* chunk = nullptr;
	};
	class SeparateChunksNormalsMergerTask : public ParallelTask<void*>
	{
	public:
		SeparateChunksNormalsMergerTask(const std::vector<ImplicitFunctionChunk*>& chunks) : chunks(chunks) {}

		void process();
		void run() override;
		~SeparateChunksNormalsMergerTask() {
			t->join();
			delete t;
		}
	private:
		std::thread* t;
		std::vector<ImplicitFunctionChunk*> chunks;
	} *merger_task = nullptr;
	void setShader(Shader* shader) override;
	void setNormalShader(Shader* shader) override;
	void update(float dt) override;
	void render() override;
	std::vector<ImplicitFunctionChunkTask*> tasks;
	std::vector<ImplicitFunctionChunk*> chunks;
};
#endif //OPENGLPROJECT_BOUNDBOX_H
