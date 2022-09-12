#version 410 core
layout (location = 0) in vec3 location_position;
layout (location = 1) in vec3 location_normal;
layout (location = 2) in vec2 location_tex;
out vec3 from_vert_normal;
out vec2 from_vert_texcoord;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_PointSize = 5.0;
    from_vert_normal = location_normal;
    from_vert_texcoord = location_tex;
    gl_Position =
        projection*
        view*
        vec4(location_position, 1.0);
}