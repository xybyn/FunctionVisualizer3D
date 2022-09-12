//#include <iostream>
//#include "FreeCamera.h"
//
//using namespace glm;
//
//#include "../imgui/imgui.h"
//#include "../imgui/imgui_impl_glfw.h"
//#include "../imgui/imgui_impl_opengl3.h"
//
//namespace {
//    glm::vec3 camera_position = vec3(0, 0, 5);
//    glm::vec3 camera_direction = vec3(1, 0, -1);
//    float yaw_angle = 180;
//    GLFWwindow *window;
//    float pitch_angle = 0;
//
//    bool moving_forward = false;
//    bool moving_backward = false;
//    bool moving_left = false;
//    bool moving_right = false;
//
//
//
//}
//
//glm::mat4 FreeCamera::getView() const{
//
//    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
//    glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));
//    glm::vec3 camera_up = glm::cross(camera_direction, camera_right);
//    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//    return glm::lookAt(camera_position, camera_position + camera_direction, vec3(0, 1, 0));
//}
//
//glm::vec3 FreeCamera::getPosition() const{
//    return camera_position;
//}
//
//void FreeCamera::update(float dt) {
//    int display_w, display_h;
//    glfwGetWindowSize(window, &display_w, &display_h);
//    projection = glm::perspective(45.f, display_w / static_cast<float>(display_h), 0.1f, 100.0f);
//    bool moved = false;
//    vec3 moving_direction = vec3(0);
//
//    if (moving_forward) {
//        moving_direction += camera_direction;
//        moved = true;
//    }
//    if (moving_backward) {
//        moving_direction -= camera_direction;
//        moved = true;
//
//    }
//    if (moving_left) {
//        moving_direction -= cross(camera_direction, vec3(0, 1, 0));
//        moved = true;
//    }
//
//    if (moving_right) {
//        moving_direction += cross(camera_direction, vec3(0, 1, 0));
//        moved = true;
//    }
//    if (moved)
//        camera_position += normalize(moving_direction) * 0.05f;
//}
//
//FreeCamera::FreeCamera(GLFWwindow *glfw_window, const InputController *controller) : CameraBase(glfw_window), controller(controller){
//    int display_w, display_h;
//    glfwGetWindowSize(glfw_window, &display_w, &display_h);
//    projection = glm::perspective(45.f, display_w / static_cast<float>(display_h), 0.1f, 100.0f);
//    window = glfw_window;
//}
//
//glm::mat4 FreeCamera::getProjection() const{
//    return projection;
//}
//
//glm::vec3 FreeCamera::getDirection() const{
//    return camera_direction;
//}
//