#version 430 core
layout (location = 0) in vec3 location_position;
void main()
{
    gl_Position = vec4(location_position, 1.0);
}
