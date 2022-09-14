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

    void setWorldPosition(const glm::vec3 &new_world_position);

    void setShader(Shader *shader)
    {
        this->shader = shader;
    }

    void setTexture(Texture *texture)
    {
        this->texture = texture;
    }

    void setNormalShader(Shader *normal_shader)
    {
        this->normals_shader = normal_shader;
    }

protected:
    GLuint VAO;
    GLuint EBO;
    GLuint VBO;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<glm::vec3> tangents;
    std::vector<uint> indices;
    Shader *shader = nullptr;
    Shader *normals_shader = nullptr;
    Texture *texture = nullptr;

    void initialize_buffers();

private:
    glm::mat4 translate_matrix;

};


#endif //OPENGLPROJECT_WORLDOBJECT_H
