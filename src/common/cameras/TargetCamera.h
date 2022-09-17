//
// Created by pc on 28.07.22.
//

#ifndef OPENGLPROJECT_TARGETCAMERA_H
#define OPENGLPROJECT_TARGETCAMERA_H


#include "CameraBase.h"
#include "common/InputController.h"

class TargetCamera : public CameraBase {
public:
    TargetCamera(GLFWwindow *window, const glm::vec3 &target_point, InputController *controller);
    void update(float dt) override;

private:
    glm::vec3 target_point;
    float phi = 45.0f;
    float ksi = 45.0f;
    float speed = 50.0f;
    float zoom_speed = 1.0f;
    float distance_to_target = 5;
    InputController *controller = nullptr;
};


#endif //OPENGLPROJECT_TARGETCAMERA_H
