//
// Created by pc on 28.07.22.
//

#ifndef OPENGLPROJECT_CAMERABASE_H
#define OPENGLPROJECT_CAMERABASE_H

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "GLFW/glfw3.h"
class CameraBase {
public:
    ~CameraBase();

    CameraBase(GLFWwindow *glfw_window) : window(glfw_window) {}

    const glm::mat4 &getView() const { return view; }

    const glm::mat4 &getProjection() const { return projection; }

    const glm::vec3 &getPosition() const { return position; }

    const glm::vec3 &getDirection() const { return direction; }

    virtual void update(float dt) {}

protected:
    GLFWwindow *window = nullptr;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;
    glm::vec3 direction;
};

#endif //OPENGLPROJECT_CAMERABASE_H
