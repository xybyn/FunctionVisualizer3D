//
// Created by pc on 27.07.22.
//

#ifndef OPENGLPROJECT_WORLDOBJECT_H
#define OPENGLPROJECT_WORLDOBJECT_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include "Shader.h"
#include "Texture.h"

class WorldObject
{
public:
    virtual ~WorldObject()
    {}

    WorldObject();

    virtual void render();
    virtual void build() {}
    virtual void addOnDoneEvent() {}
    virtual void addOnProcessEvent() {}

    virtual void update(float dt) {}


    void setWorldPosition(const glm::vec3 &new_world_position);

    virtual void setShader(Shader *shader)
    {
        this->shader = shader;
    }

    virtual void setTexture(Texture *texture)
    {
        this->texture = texture;
    }

    virtual void setNormalShader(Shader *normal_shader)
    {
        this->normals_shader = normal_shader;
    }
    void addChild(WorldObject* child)
    {
        children.push_back(child);
        child->setParent(this);
    }
    glm::mat4 getTransform() const {
        return transform_matrix;
    }
public:
    GLuint VAO;
    GLuint EBO;
    GLuint VBO;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> tangents;
    std::vector<GLuint> indices;
    Shader *shader = nullptr;
    Shader *normals_shader = nullptr;
    Texture *texture = nullptr;

    void initialize_buffers();
protected:
    void setParent(WorldObject* parent)
    {
        this->parent = parent;
    }
    glm::mat4 transform_matrix = glm::mat4(1);
    WorldObject* parent = nullptr;

private:
    std::vector<WorldObject*> children;
};


#endif //OPENGLPROJECT_WORLDOBJECT_H
