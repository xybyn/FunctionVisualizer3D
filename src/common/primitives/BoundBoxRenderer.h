#ifndef OPENGLPROJECT_BOUNDBOXRENDERER_H
#define OPENGLPROJECT_BOUNDBOXRENDERER_H

#include "common/WorldObject.h"
#include "common/abstracts/BoundBox.h"

class BoundBoxRenderer : public WorldObject
{
private:
    BoundBox bb;
public:
    BoundBoxRenderer(const BoundBox &bb);
    void render() override;
};

#endif //OPENGLPROJECT_BOUNDBOXRENDERER_H
