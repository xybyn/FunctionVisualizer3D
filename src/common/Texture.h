/** 
 * Copyright (C) 2018 Tomasz Gałaj
 **/

#pragma once
#include <string>
#include <gl/glew.h>
#include <map>

class Texture
{
public:
    Texture();
    ~Texture();

    bool load(int index, const std::string & file_name);
    
    void bind(int index = 0) const;
private:
    std::map<int, GLuint> indices;
};
