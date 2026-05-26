#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform CameraData
{
    mat4 u_ViewProjection;
    vec4 u_CameraPos;
};

layout(std140, binding = 1) uniform ObjectData
{
    mat4 u_Transform;
    mat4 u_NormalMatrix;
    vec4 u_AlbedoMetallic;
    vec4 u_RoughnessAoPad;
    ivec4 u_EntityIDPad;
};

layout(location = 0) out vec3 v_WorldPos;
layout(location = 1) out vec3 v_Normal;

void main()
{
    vec4 worldPos = u_Transform * vec4(a_Position, 1.0);
    v_WorldPos = worldPos.xyz;
    v_Normal = normalize(mat3(u_NormalMatrix) * a_Normal);
    gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int pickingID;

layout(location = 0) in vec3 v_WorldPos;
layout(location = 1) in vec3 v_Normal;

struct DirectionalLight
{
    vec4 DirectionIntensity;
    vec4 ColorPad;
};

struct PointLight
{
    vec4 PositionRadius;
    vec4 ColorIntensity;
};
const int MAX_POINT_LIGHTS = 32;

struct IBLData
{
    vec4 DiffuseColor;
    vec4 SpecularColor;
};

layout(std140, binding = 0) uniform CameraData
{
    mat4 u_ViewProjection;
    vec4 u_CameraPos;
};

layout(std140, binding = 1) uniform ObjectData
{
    mat4 u_Transform;
    mat4 u_NormalMatrix;
    vec4 u_AlbedoMetallic;
    vec4 u_RoughnessAoPad;
    ivec4 u_EntityIDPad;
};

layout(std140, binding = 2) uniform LightingData
{
    DirectionalLight u_Light;
    ivec4 u_PointLightCountPad;
    PointLight u_PointLights[MAX_POINT_LIGHTS];
    IBLData u_IBL;
};

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / max(PI * denom * denom, 0.000001);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float denom = NdotV * (1.0 - k) + k;
    return NdotV / max(denom, 0.000001);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    return GeometrySchlickGGX(NdotV, roughness) * GeometrySchlickGGX(NdotL, roughness);
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
    vec3 N = normalize(v_Normal);
    vec3 V = normalize(u_CameraPos.xyz - v_WorldPos);
    vec3 L = normalize(-u_Light.DirectionIntensity.xyz);
    vec3 H = normalize(V + L);

    vec3 u_Albedo = u_AlbedoMetallic.xyz;
    float u_Metallic = u_AlbedoMetallic.w;
    float u_Roughness = u_RoughnessAoPad.x;
    float u_AO = u_RoughnessAoPad.y;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, u_Albedo, u_Metallic);

    float NDF = DistributionGGX(N, H, max(u_Roughness, 0.045));
    float G = GeometrySmith(N, V, L, max(u_Roughness, 0.045));
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = numerator / max(denominator, 0.000001);

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - u_Metallic);

    float NdotL = max(dot(N, L), 0.0);
    vec3 radiance = u_Light.ColorPad.xyz * u_Light.DirectionIntensity.w;
    vec3 Lo = (kD * u_Albedo / PI + specular) * radiance * NdotL;

    int pointCount = clamp(u_PointLightCountPad.x, 0, MAX_POINT_LIGHTS);
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        if (i >= pointCount) break;
        vec3 toLight = u_PointLights[i].PositionRadius.xyz - v_WorldPos;
        float dist = length(toLight);
        if (dist > u_PointLights[i].PositionRadius.w)
            continue;

        vec3 LP = normalize(toLight);
        vec3 HP = normalize(V + LP);
        float atten = 1.0 - clamp(dist / max(u_PointLights[i].PositionRadius.w, 0.0001), 0.0, 1.0);
        atten *= atten;

        float NDFp = DistributionGGX(N, HP, max(u_Roughness, 0.045));
        float Gp = GeometrySmith(N, V, LP, max(u_Roughness, 0.045));
        vec3 Fp = FresnelSchlick(max(dot(HP, V), 0.0), F0);
        vec3 specp = (NDFp * Gp * Fp) / max(4.0 * max(dot(N, V), 0.0) * max(dot(N, LP), 0.0), 0.000001);
        vec3 kSp = Fp;
        vec3 kDp = (vec3(1.0) - kSp) * (1.0 - u_Metallic);
        vec3 radianceP = u_PointLights[i].ColorIntensity.xyz * u_PointLights[i].ColorIntensity.w * atten;
        float NdotLP = max(dot(N, LP), 0.0);
        Lo += (kDp * u_Albedo / PI + specp) * radianceP * NdotLP;
    }

    vec3 ambient = (u_IBL.DiffuseColor.xyz * u_Albedo + u_IBL.SpecularColor.xyz * F0) * u_AO;
    vec3 finalColor = ambient + Lo;
    finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor, vec3(1.0 / 2.2));
    color = vec4(finalColor, 1.0);
    pickingID = u_EntityIDPad.x;
}
