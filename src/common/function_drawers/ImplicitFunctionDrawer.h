#ifndef OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H
#define OPENGLPROJECT_IMPLICITFUNCTIONDRAWER_H

#include "common/WorldObject.h"
#include "common/abstracts/BoundBox.h"
#include "common/tasks/ParallelTask.h"

typedef std::function<float(const glm::vec3&)> SDF;

struct ImplicitFunctionConfiguration
{
	const char* description;
	SDF f;
	glm::vec3 steps = glm::vec3(0.125);
	BoundBox volume = BoundBox(glm::vec3(-6), glm::vec3(6));
	bool invert_normals = true;
	bool postpone = true;
};

class ImplicitFunctionChunk : public WorldObject
{
public:
	ImplicitFunctionChunk(const ImplicitFunctionConfiguration& config);


	struct ThreadResult {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<GLuint> indices;

	};
	ImplicitFunctionConfiguration config;
	void calculate_parallel(int count_of_threads, std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals, std::vector<GLuint>& indices,
		const glm::vec3& step, const BoundBox& bound, SDF function);

	void get_volume_vertices_normals_indices();

	friend class ImplicitFunctionChunkTask;
};

class ImplicitFunctionDrawer : public WorldObject
{
public:
	ImplicitFunctionDrawer(const ImplicitFunctionConfiguration& config);
	class ImplicitFunctionChunkTask : public ParallelTask<void*>
	{
	public:
		ImplicitFunctionChunkTask(ImplicitFunctionChunk* chunk) : chunk(chunk) {}

		void process() override;
	private:
		ImplicitFunctionChunk* chunk = nullptr;
	};
	void setShader(Shader* shader) override;
	void build() override;
	void setNormalShader(Shader* shader) override;
	void update(float dt) override;
	void render() override;
	ImplicitFunctionChunkTask* task = nullptr;
	ImplicitFunctionChunk* chunk = nullptr;
private:
	void on_done(void*);
};
#endif //OPENGLPROJECT_BOUNDBOX_H
