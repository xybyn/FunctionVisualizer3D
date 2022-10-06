#ifndef OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H
#define OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H

#include "common/WorldObject.h"
#include "common/abstracts/BoundBox.h"
#include "common/tasks/ParallelTask.h"
typedef float (*SDF)(const glm::vec3&);
class ImplicitFunctionChunk : public WorldObject
{
public:
	ImplicitFunctionChunk(SDF function, const glm::vec3& step, const BoundBox& bound, bool inverted = false);


	struct ThreadResult {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<GLuint> indices;

	};
	SDF function;
	glm::vec3 step;
	BoundBox bound;
	bool inverted;
	void calculate_parallel(int count_of_threads, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<GLuint>& indices,
		const glm::vec3& step, const BoundBox& bound, SDF function);

	void get_volume_vertices_normals_indices();

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

		void process() override;
	private:
		ImplicitFunctionChunk* chunk = nullptr;
	};
	void setShader(Shader* shader) override;
	void setNormalShader(Shader* shader) override;
	void update(float dt) override;
	void render() override;
	ImplicitFunctionChunkTask* task = nullptr;
	ImplicitFunctionChunk* chunk = nullptr;
private:
	void on_done(void*);
};
#endif //OPENGLPROJECT_BOUNDBOX_H
