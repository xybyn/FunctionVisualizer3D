#ifndef OPENGLPROJECT_BOUNDBOX_H
#define OPENGLPROJECT_BOUNDBOX_H

#include "common/WorldObject.h"

class BoundBox : public WorldObject
{
private:
    glm::vec3 left_lower_back;
    glm::vec3 right_upper_front;

public:
    BoundBox(const glm::vec3 &left_lower_back, const glm::vec3 &right_upper_front);

    float getSizeX() const;
    float getSizeY() const;
    float getSizeZ() const;

    void render() override;
};

#endif //OPENGLPROJECT_BOUNDBOX_H