#version 410 core
layout (location = 0) in vec3 location_position;
layout (location = 1) in vec3 location_normal;

out vec3 from_vert_light_intensity;

uniform vec4 uniform_light_position;
uniform vec3 uniform_Kd;
uniform vec3 uniform_Ld;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec3 tnorm = location_normal;
    vec4 eye_coords = view * vec4(location_position, 1);
    vec3 s = normalize(vec3(uniform_light_position - eye_coords));
    from_vert_light_intensity = uniform_Kd * uniform_Ld * max(dot(s, tnorm), 0.0);

    gl_Position =
    projection*
    view*
    vec4(location_position, 1.0);
}