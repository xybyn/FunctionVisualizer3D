#version 410 core
layout (location = 0) in vec3 location_position;
layout (location = 1) in vec3 location_normal;
layout (location = 2) in vec2 location_tex;
out vec3 from_vert_light_intensity;
out vec2 from_vert_texcoord;
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
    vec3 tnorm = location_normal;
    vec4 eye_coords = view * vec4(location_position, 1);
    vec3 s = normalize(vec3(uniform_light.position - eye_coords));
    vec3 v = normalize(-eye_coords.xyz);
    vec3 r = reflect(-s, tnorm);
    vec3 ambient = uniform_light.La * uniform_material.Ka;
    float sDotN = max(dot(s, tnorm), 0.0);
    vec3 diffuse = uniform_light.Ld * uniform_material.Kd * sDotN;
    vec3 spec = vec3(0.0);
    if(sDotN > 0.0)
    spec = uniform_light.Ls * uniform_material.Ks * pow(max(dot (r, v), 0.0), uniform_material.shininess);

    from_vert_light_intensity = ambient + diffuse + spec;
    from_vert_texcoord = location_tex;
    gl_Position =
    projection*
    view*
    vec4(location_position, 1.0);
}