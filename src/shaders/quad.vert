#version 420 core
layout (location = 0) in vec3 location_position;
layout (location = 2) in vec2 location_tex;
out vec2 from_vert_texcoord;
out vec3 from_vert_position;

void main()
{
    from_vert_texcoord = location_tex;
    gl_Position = vec4(location_position, 1.0);
}