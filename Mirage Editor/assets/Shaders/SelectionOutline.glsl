#type vertex
#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position, 0.0, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec2 v_TexCoord;

layout(binding = 0) uniform sampler2D u_SceneColor;
layout(binding = 1) uniform isampler2D u_EntityIDTex;

layout(std140, binding = 7) uniform OutlineData
{
    ivec4 u_SelectionCountPad;
    ivec4 u_SelectedEntityIDs[128];
    vec4 u_TexelSizePad;
    vec4 u_OutlineColor;
};

int sampleID(vec2 uv)
{
    return texture(u_EntityIDTex, uv).r;
}

bool isSelectedID(int id)
{
    int count = clamp(u_SelectionCountPad.x, 0, 128);
    for (int i = 0; i < count; i++)
    {
        if (u_SelectedEntityIDs[i].x == id)
            return true;
    }
    return false;
}

void main()
{
    vec4 base = texture(u_SceneColor, v_TexCoord);
    int selectedCount = clamp(u_SelectionCountPad.x, 0, 128);
    vec2 u_TexelSize = u_TexelSizePad.xy;
    float u_OutlineWidth = max(u_TexelSizePad.z, 1.0);
    if (selectedCount <= 0)
    {
        color = base;
        return;
    }

    int id = sampleID(v_TexCoord);
    bool isSelected = isSelectedID(id);
    bool edge = false;

    // Outer silhouette only: draw strictly on background pixels adjacent to selected pixels.
    if (!isSelected && id == -1)
    {
        vec2 stepX = vec2(u_TexelSize.x * u_OutlineWidth, 0.0);
        vec2 stepY = vec2(0.0, u_TexelSize.y * u_OutlineWidth);
        int n0 = sampleID(v_TexCoord + stepX);
        int n1 = sampleID(v_TexCoord - stepX);
        int n2 = sampleID(v_TexCoord + stepY);
        int n3 = sampleID(v_TexCoord - stepY);
        edge = isSelectedID(n0) || isSelectedID(n1) || isSelectedID(n2) || isSelectedID(n3);
    }

    if (edge)
    {
        float alpha = 0.9;
        color = mix(base, u_OutlineColor, alpha);
    }
    else
    {
        color = base;
    }
}
