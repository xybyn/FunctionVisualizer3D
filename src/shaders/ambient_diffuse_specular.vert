#version 410 core
layout (location = 0) in vec3 location_position;
layout (location = 1) in vec3 location_normal;
layout (location = 2) in vec2 location_tex;
out vec2 from_vert_texcoord;
out vec3 from_vert_normal;
out vec3 from_vert_position;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 lightSpaceMatrix;

void main()
{
    from_vert_texcoord = location_tex;
    from_vert_normal = location_normal;
    from_vert_position = location_position;
    FragPosLightSpace= lightSpaceMatrix * vec4(location_position, 1);
    gl_PointSize = 5.0;
    gl_Position =
    projection*
    view*
    vec4(location_position, 1.0);
}
