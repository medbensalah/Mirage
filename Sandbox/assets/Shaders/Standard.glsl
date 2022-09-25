#type vertex
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in vec2 a_Tiling;
layout(location = 5) in vec2 a_Offset;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out vec2 v_Tiling;
out vec2 v_Offset;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_Tiling = a_Tiling;
    v_Offset = a_Offset;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec2 v_Tiling;
in vec2 v_Offset;

uniform sampler2D u_Textures[32];

void main()
{
    vec2 uv = v_TexCoord * v_Tiling + v_Offset;
    color = texture(u_Textures[int(v_TexIndex)], uv) * v_Color;
}