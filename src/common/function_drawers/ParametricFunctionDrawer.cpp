#include "ParametricFunctionDrawer.h"
#include <vector>
#include "common/Utils.h"
#include <limits>

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

    vector<uint> get_indices(int horizontalDivisions, int verticalDivisions)
    {
        vector<uint> indices;
        for (int i = 0; i < verticalDivisions - 1; i++)
            for (int j = 0; j < horizontalDivisions - 1; j++)
            {
                indices.push_back((uint) (i * horizontalDivisions + j));
                indices.push_back((uint) (i * horizontalDivisions + j + 1));
                indices.push_back((uint) ((i + 1) * horizontalDivisions + j));

                indices.push_back((uint) (i * horizontalDivisions + j + 1));
                indices.push_back((uint) ((i + 1) * horizontalDivisions + j + 1));
                indices.push_back((uint) ((i + 1) * horizontalDivisions + j));
            }

        return indices;
    }

}

ParametricFunctionDrawer::ParametricFunctionDrawer(DoubleVariableFunction x, DoubleVariableFunction y,
                                                   DoubleVariableFunction z, const vec2 &u_range, const vec2 &v_range,
                                                   const ivec2 &divisions, bool inverted)
{
    //tree = new CubeTree<vec3>();
    float su = (u_range.y - u_range.x) / (divisions.x - 1);
    float sv = (v_range.y - v_range.x) / (divisions.y - 1);
    vec3 min_corner(INFINITY), max_corner(-INFINITY);
    for (int i = 0; i < divisions.x; ++i)
    {
        float u = u_range.x + i * su;
        for (int j = 0; j < divisions.y; ++j)
        {
            float v = v_range.x + j * sv;
            const glm::vec3 &vertex = vec3(x(u, v), y(u, v), z(u, v));

            vertices.push_back(vertex);
            min_corner.x = glm::min(min_corner.x, vertex.x);
            min_corner.y = glm::min(min_corner.y, vertex.y);
            min_corner.z = glm::min(min_corner.z, vertex.z);

            max_corner.x = glm::max(max_corner.x, vertex.x);
            max_corner.y = glm::max(max_corner.y, vertex.y);
            max_corner.z = glm::max(max_corner.z, vertex.z);
        }
    }
    bb = new BoundBox(min_corner, max_corner);
    tree = new CubeTree<int>(*bb);

    indices = get_indices(divisions.x, divisions.y);
    normals = calculate_normals(divisions.x, divisions.y, vertices, inverted ? -1 : 1);
    for (int i = 0; i < vertices.size(); i++)
    {
        tree->insert(CubeTreeUnit<int>(vertices[i], i));
    }

    int height = divisions.x;
    int width = divisions.y;
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (i == 0 || j == 0 || i == height - 1 || j == height - 1)
            {
                const vec3 &p = vertices[i * height + j];
                vector<CubeTreeUnit<int>> points_around_p;
                auto bound = BoundBox(p - vec3(0.1f), p + vec3(0.1f));
                tree->getData(bound, points_around_p);

                for (int k = 0; k < points_around_p.size(); ++k)
                {
                    if (are_points_same(p, points_around_p[k].getPosition()))
                    {
                        int index = points_around_p[k].getData();
                        vec3 avg = normalize(normals[i * height + j] + normals[index]);
                        vertices[i * height + j] = points_around_p[k].getPosition();
                        normals[i * height + j] = avg;
                        normals[index] = avg;
                    }
                }
            }

        }
    }
    initialize_buffers();
}

