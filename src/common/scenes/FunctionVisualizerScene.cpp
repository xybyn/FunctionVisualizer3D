#include "FunctionVisualizerScene.h"
#include <vector>
#include "common/primitives/BoundBoxRenderer.h"
#include "common/trees/CubeTree.h"
#include "common/trees/CubeTreeRenderer.h"
#include <common/primitives/Sphere.h>
#include "common/Texture.h"
#include "common/primitives/Point.h"
#include "common/shaders/NormalsShader.h"
#include "common/primitives/Plane.h"

using namespace std;
using namespace glm;

void FunctionVisualizerScene::update(float dt) {
    Scene::update(dt);
}

float f(const vec3 &v) {
    float x = v.x;
    float y = v.y;
    float z = v.z;
    //return sin(x*20);
    float a = 0.0f, b = -5.0f, c = 11.8f;
    return x * x * x * x + y * y * y * y + z * z * z * z + a * (x * x + y * y + z * z) * (x * x + y * y + z * z) +
           b * (x * x + y * y + z * z) + c;
}

float f1(const vec3 &v) {
    return v.z;
}
float R = 1;
float r = 0.5;

float x0(float u, float v) {
    return (R + r * cos(u)) * cos(v);
}

float y0(float u, float v) {
    return (R + r * cos(u)) * sin(v);
}

float z0(float u, float v) {
    return r * sin(u);
}

float x1(float u, float v) {
    return cos(u) * cos(v);
}

float y1(float u, float v) {
    return sin(u) * cos(v);
}

float z1(float u, float v) {
    return sin(v);
}

float min_x(float u, float v) {
    return fmax(x0(u, v), x1(u, v));
}

float min_y(float u, float v) {
    return fmax(y0(u, v), y1(u, v));
}

float min_z(float u, float v) {
    return fmax(z0(u, v), z1(u, v));
}

FunctionVisualizerScene::FunctionVisualizerScene(CameraBase *camera) : Scene(camera) {
        auto p0 = vec3(-5.5f);
    auto p1 = vec3(5.5);
    bb_shader = new DefaultShader(camera, ROOT_DIR "src/shaders/bound_box.vert", ROOT_DIR "src/shaders/bound_box.frag");
    BoundBox b(p0, p1);

    BoundBox a(vec3(-1), vec3(1));

    auto *tree = new CubeTree<int>(a);
    tree->insert(CubeTreeUnit<int>(vec3(0), 0));
    tree->insert(CubeTreeUnit<int>(vec3(0.25), 1));
    tree->insert(CubeTreeUnit<int>(vec3(0.5), 2));

    float pi = glm::pi<float>();
    parametric_function_drawer = new ParametricFunctionDrawer(x0, y0, z0, vec2(0.0f, 2 * pi), vec2(0, pi*2), ivec2(80, 80));
    parametric_function_drawer->setShader(function_shader);


    Plane *plane = new Plane(5, 5);
    plane->setShader(function_shader);
    bb = new BoundBoxRenderer(a);
    bb->setShader(bb_shader);

    NormalsShader *normals_shader = new NormalsShader(camera);

    BoundBoxRenderer *renderer = new BoundBoxRenderer(*parametric_function_drawer->bb);
    CubeTreeRenderer<int> *tree_renderer = new CubeTreeRenderer<int>(parametric_function_drawer->tree);
    //parametric_function_drawer->setNormalShader(normals_shader);

    tree_renderer->setShader(bb_shader);
    renderer->setShader(bb_shader);

    //add(parametric_function_drawer);
    //add(renderer);
    //add(parametric_function_drawer);
    //add(tree_renderer);

    function_shader = new AmbientDiffuseSpecularShader(camera);
    implicit_function_drawer = new ImplicitFunctionDrawer(f, vec3(0.05), b);

    flat_shader = new FlatADSShader(camera);
    implicit_function_drawer->setShader(function_shader);
    implicit_function_drawer->setNormalShader(normals_shader);
    add(implicit_function_drawer);
    //add(plane);

}
