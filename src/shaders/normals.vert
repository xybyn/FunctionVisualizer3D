#version 410 core
layout (location = 0) in vec3 location_position;
layout (location = 1) in vec3 location_normal;
layout (location = 2) in vec2 location_tex;
out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection*
    view*
    vec4(location_position, 1.0);

    mat3 normal_matrix = mat3(transpose(inverse(view)));
    vs_out.normal = normalize(vec3(projection * vec4(normal_matrix * location_normal, 0.0)));
}