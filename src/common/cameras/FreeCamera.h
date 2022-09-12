//#ifndef OPENGLPROJECT_FREECAMERA_H
//#define OPENGLPROJECT_FREECAMERA_H
//
//#include <glm/common.hpp>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <GLFW/glfw3.h>
//#include "CameraBase.h"
//#include "InputController.h"
//
//class FreeCamera : public CameraBase
//{
//public:
//	glm::mat4 getView()  const;
//	glm::mat4 getProjection() const;
//	glm::vec3 getPosition() const;
//	glm::vec3 getDirection() const;
//	void update(float dt);
//	FreeCamera(GLFWwindow *window, const InputController *controller);
//    bool cursor_hidden = true;
//
//private:
//	glm::mat4 projection;
//    const InputController *controller;
//};
//
//#endif //OPENGLPROJECT_FREECAMERA_H