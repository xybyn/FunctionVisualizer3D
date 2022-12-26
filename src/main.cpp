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
#include "common/tasks/HelloWorldTask.h"

using namespace std;
using namespace glm;

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
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
    glEnable(GL_MULTISAMPLE);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 420");
    auto font = io.Fonts->AddFontFromFileTTF(ROOT_DIR "resources/fonts/NotoSans-Regular.ttf", 20);
   // auto font = io.Fonts->AddFontDefault();
    //ImGui::PushFont(font);
    InputController *controller = new InputController(window);
    TargetCamera *camera = new TargetCamera(window, vec3(0), controller);
    int selected_scene_index = 0;

    Scene* scene=new FunctionVisualizerScene(camera);
   
    HelloWorldTask *task = new HelloWorldTask("hello world");
    //task->run();
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        static double prev_time = 0;
        double current_time = glfwGetTime();
        double dt = current_time - prev_time;
        prev_time = current_time;
        if (task)
        {
            if (task->isInProgress())
            {
                //cout << "task in progress" << endl;
                //cout<<task->getProgress()<<endl;
            }
            if (task->isDone())
            {
               // cout << "task is done, result: " << task->getResult()<< endl;
                delete task;
                task = nullptr;
            }

        }

        
        controller->update(0);
        scene->update(dt);
        scene->render();
        //ImGui::SetNextWindowPos(ImVec2(0, 0));
        //if (ImGui::Begin("New Window"))
        //{
        //    ImGui::Combo("Scenes", &selected_scene_index, &scene_names[0], scene_names.size());
        //
        //    scenes[selected_scene_index]->update(dt);
        //    scenes[selected_scene_index]->render();
        //    ImGui::End();
        //}
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

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
