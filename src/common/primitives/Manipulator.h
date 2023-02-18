#ifndef OPENGLPROJECT_MANIPULATOR_H
#define OPENGLPROJECT_MANIPULATOR_H

#include "common/WorldObject.h"
#include "common/abstracts/BoundBox.h"
#include "common/primitives/Line.h"
#include "common/primitives/Plane.h"
#include "common/primitives/Triangle.h"
#include "common/cameras/CameraBase.h"
class Manipulator : public WorldObject
{
public:
	Manipulator(CameraBase *camera);
	void render() override;
private:
	Line* x_line = nullptr;
	Line* y_line = nullptr;
	Line* z_line = nullptr;

	Triangle* xy_plane = nullptr;
	Triangle* yz_plane = nullptr;
	Triangle* xz_plane = nullptr;
};

#endif //OPENGLPROJECT_BOUNDBOXRENDERER_H
