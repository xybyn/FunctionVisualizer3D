//
// Created by pc on 28.07.22.
//

#include "InputController.h"
#include "imgui/imgui_impl_glfw.h"

InputController *InputController::instance = nullptr;

float InputController::getMovingX() const {
    return moving_x;
}

float InputController::getMovingY() const {
    return moving_y;
}

float InputController::getCameraMovingDX() const {

    static bool first_time = true;
    static float prev_x = 0.0f;
    static float prev_y = 0.0f;
    if (first_time) {
        first_time = false;
    } else {
        instance->camera_moving_dx = camera_moving_x - prev_x;
        instance->camera_moving_dy = camera_moving_y - prev_y;
    }


    prev_x = camera_moving_x;
    prev_y = camera_moving_y;

    return camera_moving_dx;
}

float InputController::getCameraMovingDY() const {
    return camera_moving_dy;
}

float InputController::getScrollingDY() const {
    return scroll_dy;
}

InputController::InputController(GLFWwindow *window) {
    instance = this;
    glfwSetCursorPosCallback(window, InputController::mouse_callback);
    glfwSetKeyCallback(window, InputController::key_callback);
    glfwSetMouseButtonCallback(window, InputController::mouse_button_callback);
    glfwSetScrollCallback(window, InputController::scroll_callback);
}

void InputController::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mode);
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS)
            instance->moving_y = 1.0f;
        else if (action == GLFW_RELEASE)
            instance->moving_y = 0.f;
    }
    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS)
            instance->moving_y = -1.0f;
        else if (action == GLFW_RELEASE)
            instance->moving_y = 0.0f;
    }
    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS)
            instance->moving_y = -1.0f;
        else if (action == GLFW_RELEASE)
            instance->moving_y = 0.0f;
    }
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS)
            instance->moving_y = 1.0f;
        else if (action == GLFW_RELEASE)
            instance->moving_y = 0.0f;
    }
}

void InputController::mouse_callback(GLFWwindow *window, double x_pos, double y_pos) {
    ImGui_ImplGlfw_CursorPosCallback(window, x_pos, y_pos);
    instance->camera_moving_x = static_cast<float>(x_pos);
    instance->camera_moving_y = static_cast<float>(y_pos);
}

bool InputController::isRMBPressed() const {
    return rmb_pressed;
}

void InputController::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    instance->scroll_dy = static_cast<float>(yoffset);
}

void InputController::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    instance->rmb_pressed = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;
    instance->lmb_pressed = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
    instance->mmb_pressed = button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS;
}

float InputController::getCameraMovingX() const {
    return camera_moving_x;
}

float InputController::getCameraMovingY() const {
    return camera_moving_y;
}

void InputController::update(float dt) {
}

bool InputController::isLMBPressed() const {
    return lmb_pressed;
}

bool InputController::isMMBPressed() const {
    return mmb_pressed;
}

