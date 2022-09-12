#version 420 core

out vec4 outColor;
uniform sampler2D tex_sampler_0;
uniform sampler2D tex_sampler_1;
in vec3 from_vert_normal;
in vec3 from_vert_position;
in vec2 from_vert_texcoord;


struct LightInfo{
    vec4 position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
};

struct MaterialInfo{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
};


uniform LightInfo uniform_light;
uniform MaterialInfo uniform_material;
uniform mat4 projection;
uniform mat4 view;
void main()
{
    vec3 tnorm = from_vert_normal;
    vec4 eye_coords = view * vec4(from_vert_position, 1);
    vec3 s = normalize(vec3(uniform_light.position - eye_coords));
    vec3 v = normalize(-eye_coords.xyz);
    vec3 r = reflect(-s, tnorm);
    vec3 ambient = uniform_light.La * uniform_material.Ka;
    float sDotN = max(dot(s, tnorm), 0.0);
    vec3 diffuse = uniform_light.Ld * uniform_material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if(sDotN > 0.0)
    spec = uniform_light.Ls * uniform_material.Ks * pow(max(dot (r, v), 0.0), uniform_material.shininess);
    vec4 tex_0_color = texture(tex_sampler_0, from_vert_texcoord);
    vec4 tex_1_color = texture(tex_sampler_1, from_vert_texcoord);
    vec4 texture_pixel_color = mix(tex_0_color, tex_1_color, tex_0_color.a);

    outColor = texture_pixel_color * vec4(ambient + diffuse + spec, 1);
}