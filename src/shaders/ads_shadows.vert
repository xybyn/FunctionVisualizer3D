#version 410 core
layout (location = 0) in vec3 location_position;
uniform mat4 lightSpaceMatrix;
void main()
{
    gl_Position = lightSpaceMatrix * vec4(location_position, 1.0);
}
