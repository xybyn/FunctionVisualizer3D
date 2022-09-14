#version 410 core
layout (location = 0) in vec3 location_position;
layout (location = 1) in vec3 location_normal;
uniform mat4 projection;
uniform mat4 view;
void main()
{
    gl_Position =
    projection*
    view*
    vec4(location_position, 1.0);
}
