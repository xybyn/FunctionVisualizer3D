#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "common/Shader.h"
#include "common/cameras/FreeCamera.h"
#include "common/primitives/Sphere.h"
#include "common/shaders/DefaultShader.h"
#include "common/shaders/DiffuseShader.h"
#include "common/shaders/AmbientDiffuseSpecularShader.h"
#include "common/Model.h"
#include "common/Texture.h"
#include "common/cameras/TargetCamera.h"
#include "common/shaders/FlatADSShader.h"
#include "common/shaders/DiscardWithPerlinShader.h"
#include "common/shaders/ToonADSShader.h"
#include "common/scenes/Scene.h"
#include "common/scenes/ToonScene.h"
#include "common/scenes/MaterialsScene.h"
#include "common/scenes/MultipleTexturesScene.h"
#include "common/scenes/NormalMapScene.h"
#include "common/scenes/FunctionVisualizerScene.h"
#include "common/primitives/Plane.h"
#include "common/primitives/Quad2D.h"

using namespace std;
using namespace glm;

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

namespace
{
    float R = 1;
    float r = 0.5;

    float x0(float u, float v)
    {
        return (R + r * cos(u)) * cos(v);
    }

    float y0(float u, float v)
    {
        return 2 + (R + r * cos(u)) * sin(v);
    }

    float z0(float u, float v)
    {
        return r * sin(u);
    }

    float x1(float u, float v)
    {
        return -(R + r * cos(u)) * cos(v);
    }

    float y1(float u, float v)
    {
        return -2 - r * sin(u);
    }

    float z1(float u, float v)
    {
        return -2 - (R + r * cos(u)) * sin(v);
    }
}

int main()
{
    glfwInit();
    glfwSetErrorCallback(glfw_error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    int display_w = 1280, display_h = 720;
    GLFWwindow *window = glfwCreateWindow(display_w, display_h, "Window", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SIZE);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glDepthFunc(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 420");
    InputController *controller = new InputController(window);
    TargetCamera *camera = new TargetCamera(window, vec3(0), controller);
    int selected_scene_index = 0;

    vector<Scene *> scenes;
    //scenes.push_back(new ToonScene(camera));
    //scenes.push_back(new MaterialsScene(camera));
    //scenes.push_back(new MultipleTexturesScene(camera));
    //scenes.push_back(new NormalMapScene(camera));
    scenes.push_back(new FunctionVisualizerScene(camera));
    vector<const char *> scene_names;
    //scene_names.push_back("Toon");
    //scene_names.push_back("Materials");
    //scene_names.push_back("Multi textures");
    //scene_names.push_back("Normal map");

    float pi = glm::pi<float>();
    auto parametric_function_drawer = new ParametricFunctionDrawer(x0, y0, z0, vec2(0.0f, 2 * pi), vec2(0, pi * 2),
                                                                   ivec2(80, 80));
    auto plane = new Plane(50, 50);
    auto shader = new DefaultShader(camera, ROOT_DIR "src/shaders/ads_shadows.vert", ROOT_DIR "src/shaders/ads_shadows.frag");
    scene_names.push_back("Function Visualizer");
    auto quad_shader = new Shader(ROOT_DIR "src/shaders/quad.vert", ROOT_DIR "src/shaders/quad.frag");
    auto ads_shader  = new AmbientDiffuseSpecularShader(camera);
    auto quad = new Quad2D();
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, display_w, display_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        static double prev_time = 0;
        double current_time = glfwGetTime();
        double dt = current_time - prev_time;
        prev_time = current_time;
        camera->update(dt);
        controller->update(0);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 20.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(vec3(ads_shader->getLight().position), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        shader->use();
        shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, display_w, display_h);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        //glCullFace(GL_FRONT);
        parametric_function_drawer->render();
        plane->render();
        //glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quad_shader->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        quad_shader->setFloat("near_plane", near_plane);
        quad_shader->setFloat("far_plane", far_plane);
        //quad->render();
        ads_shader->use();
        ads_shader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        parametric_function_drawer->render();
        plane->render();

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, depthMap);
        //renderQuad();

        //glfwGetFramebufferSize(window, &display_w, &display_h);
        //glViewport(0, 0, display_w, display_h);

        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
