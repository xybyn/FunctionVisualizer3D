//
// Created by pc on 28.07.22.
//

#ifndef OPENGLPROJECT_INPUTCONTROLLER_H
#define OPENGLPROJECT_INPUTCONTROLLER_H


#include "GLFW/glfw3.h"

class InputController {
public:
    void update(float dt);
    InputController(GLFWwindow *window);

    float getMovingX() const;

    float getMovingY() const;

    float getCameraMovingDX() const;

    float getCameraMovingDY() const;

    float getCameraMovingX() const;

    float getCameraMovingY() const;

    float getScrollingDY() const;

    bool isRMBPressed() const;
    bool isLMBPressed() const;
    bool isMMBPressed() const;

private:
    GLFWwindow *window;

    static InputController *instance;

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

    static void mouse_callback(GLFWwindow *window, double x_pos, double y_pos);

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    float moving_x = 0.0f;
    float moving_y = 0.0f;
    float camera_moving_dx = 0.0f;
    float camera_moving_dy = 0.0f;
    float camera_moving_x = 0.0f;
    float camera_moving_y = 0.0f;
    float scroll_dy = 0.0f;
    bool rmb_pressed = false;
    bool lmb_pressed = false;
    bool mmb_pressed = false;
};


#endif //OPENGLPROJECT_INPUTCONTROLLER_H
