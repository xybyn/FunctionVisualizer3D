#include "ParametricFunctionDrawer.h"
#include <vector>
#include "common/Utils.h"
#include "common/tasks/ParallelTask.h"
#include <limits>
#include <thread>

using namespace glm;
using namespace std;

namespace
{
    vector<vec3> calculate_normals(int height, int width, const vector<vec3> &vertices, float inverted = 1)
    {
        vector<vec3> normals;

        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                if (i == height - 1)
                {
                    if (j == width - 1)
                    {
                        vec3 p1 = vertices[(i * height) + j];
                        vec3 p2 = vertices[(i * height) + j - 1];
                        vec3 p3 = vertices[((i - 1) * height) + j];
                        vec3 n = normalize(inverted * cross(p2 - p1, p3 - p1));
                        normals.push_back(n);
                    } else
                    {
                        vec3 p1 = vertices[(i * height) + j];
                        vec3 p2 = vertices[((i - 1) * height) + j + 1];
                        vec3 p3 = vertices[((i - 1) * height) + j];
                        vec3 n = normalize(inverted * cross(p3 - p1, p2 - p1));
                        normals.push_back(n);
                    }
                } else if (j + 1 == width)
                {
                    vec3 p1 = vertices[(i * height) + j];
                    vec3 p2 = vertices[((i + 1) * height) + j - 1];
                    vec3 p3 = vertices[(i * height) + j - 1];

                    vec3 n = normalize(inverted * cross(p2 - p1, p3 - p1));

                    normals.push_back(n);
                } else
                {
                    vec3 p1 = vertices[(i * height) + j];
                    vec3 p2 = vertices[(i * height) + j + 1];
                    vec3 p3 = vertices[((i + 1) * height) + j];

                    vec3 n = normalize(inverted * cross(p2 - p1, p3 - p1));

                    normals.push_back(n);
                }
            }
        }
        return normals;
    }

    vector<GLuint> get_indices(int horizontalDivisions, int verticalDivisions)
    {
        vector<GLuint> indices;
        for (int i = 0; i < verticalDivisions - 1; i++)
            for (int j = 0; j < horizontalDivisions - 1; j++)
            {
                indices.push_back((GLuint) (i * horizontalDivisions + j));
                indices.push_back((GLuint) (i * horizontalDivisions + j + 1));
                indices.push_back((GLuint) ((i + 1) * horizontalDivisions + j));

                indices.push_back((GLuint) (i * horizontalDivisions + j + 1));
                indices.push_back((GLuint) ((i + 1) * horizontalDivisions + j + 1));
                indices.push_back((GLuint) ((i + 1) * horizontalDivisions + j));
            }

        return indices;
    }


}

ParametricFunctionDrawer::ParametricFunctionDrawer(DoubleVariableFunction func, const vec2 &u_range,
                                                   const vec2 &v_range, const ivec2 &divisions, bool inverted) : func(
        func), u_range(u_range), v_range(v_range), divisions(divisions), inverted(inverted)
{
    task = new ParametricFunctionDrawerTask(this);
    task->run();
}

void ParametricFunctionDrawer::update(float dt)
{
    WorldObject::update(dt);

    if (task)
    {
        if (task->isDone())
        {
            initialize_buffers();
            task = nullptr;
            delete task;
        }
    }


}

void ParametricFunctionDrawer::render()
{
    if (!task)
        WorldObject::render();
}

void ParametricFunctionDrawer::ParametricFunctionDrawerTask::process(ParametricFunctionDrawerTask *that)
{

    float su = (owner->u_range.y - owner->u_range.x) / (owner->divisions.x - 1);
    float sv = (owner->v_range.y - owner->v_range.x) / (owner->divisions.y - 1);
    vec3 min_corner(INFINITY), max_corner(-INFINITY);
    for (int i = 0; i < owner->divisions.x; ++i)
    {
        float u = owner->u_range.x + i * su;
        for (int j = 0; j < owner->divisions.y; ++j)
        {
            float v = owner->v_range.x + j * sv;
            const glm::vec3 &vertex = owner->func(u, v);

            owner->vertices.push_back(vertex);
            min_corner.x = glm::min(min_corner.x, vertex.x);
            min_corner.y = glm::min(min_corner.y, vertex.y);
            min_corner.z = glm::min(min_corner.z, vertex.z);

            max_corner.x = glm::max(max_corner.x, vertex.x);
            max_corner.y = glm::max(max_corner.y, vertex.y);
            max_corner.z = glm::max(max_corner.z, vertex.z);
        }
    }
    owner->bb = new BoundBox(min_corner, max_corner);
    owner->tree = new CubeTree<int>(*owner->bb);

   owner->indices = get_indices(owner->divisions.x, owner->divisions.y);
   owner->normals = calculate_normals(owner->divisions.x, owner->divisions.y, owner->vertices, owner->inverted ? -1 : 1);
    for (int i = 0; i < owner->vertices.size(); i++)
    {
        owner->tree->insert(CubeTreeUnit<int>(owner->vertices[i], i));
    }

    int height = owner->divisions.x;
    int width =owner-> divisions.y;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (i == 0 || j == 0 || i == height - 1 || j == height - 1)
            {
                const vec3 &p = owner->vertices[i * height + j];
                vector<CubeTreeUnit<int>> points_around_p;
                auto bound = BoundBox(p - vec3(0.1f), p + vec3(0.1f));
                owner->tree->getData(bound, points_around_p);

                for (int k = 0; k < points_around_p.size(); ++k)
                {
                    if (are_points_same(p, points_around_p[k].getPosition()))
                    {
                        int index = points_around_p[k].getData();
                        vec3 avg = normalize(owner->normals[i * height + j] + owner->normals[index]);
                        owner->vertices[i * height + j] = points_around_p[k].getPosition();
                        owner->normals[i * height + j] = avg;
                        owner->normals[index] = avg;
                    }
                }
            }

        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    done();
}

ParametricFunctionDrawer::ParametricFunctionDrawerTask::ParametricFunctionDrawerTask(ParametricFunctionDrawer *owner)
        : owner(owner)
{

}

void ParametricFunctionDrawer::ParametricFunctionDrawerTask::run()
{
    ParallelTask::run();
    t = new thread(&ParametricFunctionDrawer::ParametricFunctionDrawerTask::process, this, this);
}
