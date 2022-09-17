#version 420 core

out vec4 outColor;
layout(binding = 0) uniform sampler2D shadowMap;
//uniform sampler2D shadowMap;

in vec3 from_vert_normal;
in vec3 from_vert_position;
in vec2 from_vert_texcoord;
in vec4 FragPosLightSpace;

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

float calculate_shadow(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if(projCoords.z > 1.0)
    return 0;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    vec4 eye_coords = view * vec4(from_vert_position, 1);
    vec3 s = normalize(vec3(uniform_light.position - eye_coords));
    float bias = max(0.05 * (1.0 - dot(from_vert_normal, s)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow = shadow/10;
    return shadow;
}

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


    float shadow = calculate_shadow(FragPosLightSpace);

    outColor = vec4(ambient + (diffuse + spec) * (1-shadow), 1);
}
