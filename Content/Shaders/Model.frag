#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec4 FragColor;

uniform vec3 uViewPosition;
uniform vec3 uColor;
uniform sampler2D uDiffuse;

#define LIGHT_DIRECTIONAL 0
#define LIGHT_POINT       1
#define LIGHT_SPOT        2

struct Light
{
    vec3 color;     int type;
    vec3 position;  float intensity;
    vec3 direction; float range;
};

layout(std140) uniform Lights
{
    int   uLightCount;
    Light uLights[8]; // gMaxLights
};

vec3 BlinnPhong(Light light, vec3 albedo, vec3 N, vec3 V, vec3 L, float factor)
{
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 32.0);

    vec3 diffuse  = albedo * diff * light.color;
    vec3 specular = spec * light.color * 0.3;

    return (diffuse + specular) * (light.intensity * factor);
}

vec3 ApplyLight(Light light, vec3 albedo, vec3 N, vec3 V)
{
    if (light.type == LIGHT_DIRECTIONAL)
    {
        vec3 L = normalize(-light.direction);
        return BlinnPhong(light, albedo, N, V, L, 1.0);
    }

    vec3  toLight = light.position - vPosition;
    float dist    = length(toLight);
    vec3  L       = toLight / dist;
    float factor  = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

    if (light.type == LIGHT_POINT)
    {
        factor *= clamp(1.0 - dist / light.range, 0.0, 1.0);
        return BlinnPhong(light, albedo, N, V, L, factor);
    }

    float outerCos = cos(radians(light.range));
    float innerCos = cos(radians(light.range * 0.8));
    float cosTheta = dot(L, normalize(-light.direction));
    float cone     = clamp((cosTheta - outerCos) / (innerCos - outerCos), 0.0, 1.0);

    return BlinnPhong(light, albedo, N, V, L, factor * cone);
}

void main()
{
    vec3 albedo = texture(uDiffuse, vTexCoords).rgb * uColor;
    vec3 color  = albedo * 0.1;

    vec3 N = normalize(vNormal);
    vec3 V = normalize(uViewPosition - vPosition);

    for (int i = 0; i < uLightCount; i++)
    {
        color += ApplyLight(uLights[i], albedo, N, V);
    }

    FragColor = vec4(color, 1.0);
}