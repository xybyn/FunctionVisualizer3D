#include "Manipulator.h"
#include "common/shaders/SingleColorShader.h"
Manipulator::Manipulator(CameraBase* camera)
{
	x_line = new Line(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
	auto x_line_color_shader = new SingleColorShader(camera);
	x_line_color_shader->setColor(glm::vec3(1, 0, 0));
	x_line->setShader(x_line_color_shader);


	y_line = new Line(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	auto y_line_color_shader = new SingleColorShader(camera);
	y_line_color_shader->setColor(glm::vec3(0, 1, 0));
	y_line->setShader(y_line_color_shader);

	z_line = new Line(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
	auto z_line_color_shader = new SingleColorShader(camera);
	z_line_color_shader->setColor(glm::vec3(0, 0, 1));
	z_line->setShader(z_line_color_shader);

	yz_plane = new Triangle(glm::vec3(0, 0, 0), glm::vec3(0, 0.5, 0), glm::vec3(0, 0, 0.5));
	auto yz_plane_color_shader = new SingleColorShader(camera);
	yz_plane_color_shader->setColor(glm::vec3(0, 0.5, 0.5));
	yz_plane->setShader(yz_plane_color_shader);

	xy_plane = new Triangle(glm::vec3(0, 0, 0), glm::vec3(0.5, 0, 0), glm::vec3(0, 0.5, 0));
	auto xy_plane_color_shader = new SingleColorShader(camera);
	xy_plane_color_shader->setColor(glm::vec3(0.5, 0.5, 0));
	xy_plane->setShader(xy_plane_color_shader);

	 xz_plane = new Triangle(glm::vec3(0, 0, 0), glm::vec3(0.5, 0, 0), glm::vec3(0, 0, 0.5));
	 auto xz_plane_color_shader = new SingleColorShader(camera);
	 xz_plane_color_shader->setColor(glm::vec3(0.5,0, 0.5));
	 xz_plane->setShader(xz_plane_color_shader);

	addChild(x_line);
	addChild(y_line);
	addChild(z_line);
	addChild(xz_plane);
	addChild(xy_plane);
	addChild(yz_plane);

}
void Manipulator::render()
{
	x_line->render();
	y_line->render();
	z_line->render();
	xy_plane->render();
	yz_plane->render();
	xz_plane->render();
}