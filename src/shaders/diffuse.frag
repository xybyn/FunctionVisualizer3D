#version 420 core

out vec4 outColor;
in vec3 from_vert_light_intensity;
void main()
{
    outColor = vec4(from_vert_light_intensity, 1);
}