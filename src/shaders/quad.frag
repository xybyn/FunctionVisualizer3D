#version 420 core

out vec4 outColor;
uniform sampler2D tex_sampler_0;
in vec2 from_vert_texcoord;
uniform float near_plane;
uniform float far_plane;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}
void main()
{
    float depthValue = texture(tex_sampler_0, from_vert_texcoord).r;
    //vec4 tex_0_color = texture(tex_sampler_0, from_vert_texcoord);
    outColor = vec4(vec3(depthValue), 1.0);
}