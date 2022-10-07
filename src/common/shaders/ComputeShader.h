#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class ComputeShader {
public:
    virtual ~ComputeShader() {}

    GLuint program;

    inline ComputeShader(const char* compute_path)
    {
        std::string compute_code;
        std::ifstream compute_shader_file;
        compute_shader_file.exceptions(std::ifstream::badbit);
        try
        {
            compute_shader_file.open(compute_path);
            std::stringstream comp_shader_stream;
            comp_shader_stream << compute_shader_file.rdbuf();
            compute_shader_file.close();
            compute_code = comp_shader_stream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const GLchar* comp_shader_code = compute_code.c_str();
        GLuint comp;
        GLint success;
        GLchar infoLog[512];
        comp = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(comp, 1, &comp_shader_code, NULL);
        glCompileShader(comp);
        glGetShaderiv(comp, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(comp, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        program = glCreateProgram();
        glAttachShader(program, comp);
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        glDeleteShader(comp);
    }

    virtual inline void use()
    {
        glUseProgram(program);
    }

    inline void setMat4(const char* value, glm::mat4 mat)
    {
        GLuint loc = glGetUniformLocation(program, value);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    inline void setMat3(const char* value, glm::mat4 mat)
    {
        GLuint loc = glGetUniformLocation(program, value);
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    inline void setVec4(const char* value, glm::vec4 vec)
    {
        GLuint loc = glGetUniformLocation(program, value);
        glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
    }

    inline void setVec3(const char* value, glm::vec3 vec)
    {
        GLuint loc = glGetUniformLocation(program, value);
        glUniform3f(loc, vec.x, vec.y, vec.z);
    }

    inline void setVec2(const char* value, glm::vec2 vec)
    {
        GLuint loc = glGetUniformLocation(program, value);
        glUniform2f(loc, vec.x, vec.y);
    }

    inline void setFloat(const char* value, float f)
    {
        GLuint loc = glGetUniformLocation(program, value);
        glUniform1f(loc, f);
    }

    inline void setInt(const char* value, int f)
    {
        GLuint loc = glGetUniformLocation(program, value);
        glUniform1i(loc, f);
    }
};

