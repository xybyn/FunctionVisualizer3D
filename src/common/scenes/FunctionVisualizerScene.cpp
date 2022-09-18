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

void FunctionVisualizerScene::update(float dt)
{
    
    Scene::update(dt);
}

float f(const vec3 &v)
{
    float x = v.x;
    float y = v.y;
    float z = v.z;
    //return sin(x*20);
    float a = 0.0f, b = -5.0f, c = 11.8f;
    return x * x * x * x + y * y * y * y + z * z * z * z + a * (x * x + y * y + z * z) * (x * x + y * y + z * z) +
           b * (x * x + y * y + z * z) + c;
}

vec3 torus(float u, float v)
{
    float R = 1;
    float r = 0.5;

    return vec3((R + r * cos(u)) * cos(v), (R + r * cos(u)) * sin(v), r * sin(u));
}


FunctionVisualizerScene::FunctionVisualizerScene(CameraBase *camera) : Scene(camera)
{
    auto p0 = vec3(-5.5f);
    auto p1 = vec3(5.5);
    bb_shader = new DefaultShader(camera, ROOT_DIR "src/shaders/bound_box.vert", ROOT_DIR "src/shaders/bound_box.frag");
    BoundBox b(p0, p1);


    function_shader = new AmbientDiffuseSpecularShader(camera);
    implicit_function_drawer = new ImplicitFunctionDrawer(f, vec3(0.05), b, true);

    //flat_shader = new FlatADSShader(camera);
    implicit_function_drawer->setShader(function_shader);

    //BoundBox a(vec3(-1), vec3(1));


    float pi = glm::pi<float>();
    parametric_function_drawer = new ParametricFunctionDrawer(torus, vec2(0.0f, 2 * pi), vec2(0, pi * 2),
                                                            ivec2(80, 80));
    parametric_function_drawer->setShader(function_shader);

    Plane *plane = new Plane(5, 5);
    plane->setShader(function_shader);
    //bb = new BoundBoxRenderer(a);
    //bb->setShader(bb_shader);

    NormalsShader *normals_shader = new NormalsShader(camera);


    add(implicit_function_drawer);
    //add(parametric_function_drawer);
   // add(plane);

}
