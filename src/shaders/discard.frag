#version 420 core

out vec4 outColor;
layout(binding = 0) uniform sampler2D tex_sampler;
in vec3 from_vert_light_intensity;
in vec3 from_vert_normal;
in vec2 from_vert_texcoord;
uniform float uniform_discard_strength;
void main()
{
    vec4 texture_pixel_color = texture(tex_sampler, from_vert_texcoord);
    bool disc = (texture_pixel_color.r) >(1-uniform_discard_strength) ;
    if(disc)
        discard;
    outColor = vec4(from_vert_light_intensity * texture_pixel_color.rgb, 1);
}