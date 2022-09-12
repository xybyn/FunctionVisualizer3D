//
// Created by pc on 28.07.22.
//

#include "TargetCamera.h"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
using namespace glm;
using namespace std;

void TargetCamera::update(float dt) {

    static float prev_x, prev_y, prev_scroll_y;
    float current_x = controller->getCameraMovingX();
    float current_y = controller->getCameraMovingY();
    float current_scroll_y = controller->getCameraMovingY();
    float dx = controller->isRMBPressed() ? (current_x - prev_x) : 0.0f;
    float dy = controller->isRMBPressed() ? (current_y - prev_y) : 0.0f;
    distance_to_target += controller->isMMBPressed()? (current_scroll_y - prev_scroll_y) * dt * zoom_speed: 0.0f;

    prev_x = current_x;
    prev_y = current_y;
    prev_scroll_y = current_scroll_y;
    phi += dx * speed * dt;
    ksi += dy * speed * dt;

    position.x = distance_to_target * cos(glm::radians(ksi)) * cos(glm::radians(phi));
    position.z = distance_to_target * cos(glm::radians(ksi)) * sin(glm::radians(phi));
    position.y = distance_to_target * sin(glm::radians(ksi));

    view = glm::lookAt(target_point + position, target_point, vec3(0, 1, 0));
    int display_w, display_h;
    glfwGetWindowSize(window, &display_w, &display_h);
    projection = glm::perspective(45.f, display_w / static_cast<float>(display_h), 0.1f, 300.0f);
    direction = normalize(target_point - position);
}

TargetCamera::TargetCamera(GLFWwindow *window, const glm::vec3 &target_point, InputController *controller)
        : CameraBase(window), controller(controller) {
    this->target_point = target_point;
}
