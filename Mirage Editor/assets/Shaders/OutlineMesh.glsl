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

void main()
{
    vec3 n = normalize(mat3(u_NormalMatrix) * a_Normal);
    vec3 p = a_Position + n * 0.03;
    vec4 worldPos = u_Transform * vec4(p, 1.0);
    gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int pickingID;

layout(std140, binding = 1) uniform ObjectData
{
    mat4 u_Transform;
    mat4 u_NormalMatrix;
    vec4 u_AlbedoMetallic;
    vec4 u_RoughnessAoPad;
    ivec4 u_EntityIDPad;
};

void main()
{
    color = vec4(u_AlbedoMetallic.rgb, 1.0);
    pickingID = u_EntityIDPad.x;
}

