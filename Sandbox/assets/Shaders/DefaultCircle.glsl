#type vertex
#version 450 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec3 a_LocalPosition;
layout(location = 2) in float a_Thickness;
layout(location = 3) in float a_Fade;
layout(location = 4) in vec4 a_Color;
layout(location = 5) in vec2 a_TexCoord;
layout(location = 6) in float a_TexIndex;
layout(location = 7) in vec2 a_Tiling;
layout(location = 8) in vec2 a_Offset;
layout(location = 9) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
	vec2 TexCoord;
	vec2 Tiling;
	vec2 Offset;
};

layout (location = 0) out VertexOutput Output;
layout (location = 7) out flat float v_TexIndex;
layout (location = 8) out flat int v_EntityID;

void main()
{
    Output.Color = a_Color;
    Output.LocalPosition = a_LocalPosition;
    Output.Thickness = a_Thickness;
    Output.Fade = a_Fade;
    Output.TexCoord = a_TexCoord;
    Output.Tiling = a_Tiling;
    Output.Offset = a_Offset;
    
	v_TexIndex = a_TexIndex;
    v_EntityID = a_EntityID;

    gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int pickingID;

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thickness;
	float Fade;
	vec2 TexCoord;
	vec2 Tiling;
	vec2 Offset;
};

layout (location = 0) in VertexOutput Input;
layout (location = 7) in flat float v_TexIndex;
layout (location = 8) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

void main()
{
    vec2 uv = Input.TexCoord * Input.Tiling + Input.Offset;
    vec4 texColor = Input.Color;
    switch(int(v_TexIndex))
    {
        case 0:  texColor *= texture(u_Textures[ 0], uv); break;
        case 1:  texColor *= texture(u_Textures[ 1], uv); break;
        case 2:  texColor *= texture(u_Textures[ 2], uv); break;
        case 3:  texColor *= texture(u_Textures[ 3], uv); break;
        case 4:  texColor *= texture(u_Textures[ 4], uv); break;
        case 5:  texColor *= texture(u_Textures[ 5], uv); break;
        case 6:  texColor *= texture(u_Textures[ 6], uv); break;
        case 7:  texColor *= texture(u_Textures[ 7], uv); break;
        case 8:  texColor *= texture(u_Textures[ 8], uv); break;
        case 9:  texColor *= texture(u_Textures[ 9], uv); break;
        case 10: texColor *= texture(u_Textures[10], uv); break;
        case 11: texColor *= texture(u_Textures[11], uv); break;
        case 12: texColor *= texture(u_Textures[12], uv); break;
        case 13: texColor *= texture(u_Textures[13], uv); break;
        case 14: texColor *= texture(u_Textures[14], uv); break;
        case 15: texColor *= texture(u_Textures[15], uv); break;
        case 16: texColor *= texture(u_Textures[16], uv); break;
        case 17: texColor *= texture(u_Textures[17], uv); break;
        case 18: texColor *= texture(u_Textures[18], uv); break;
        case 19: texColor *= texture(u_Textures[19], uv); break;
        case 20: texColor *= texture(u_Textures[20], uv); break;
        case 21: texColor *= texture(u_Textures[21], uv); break;
        case 22: texColor *= texture(u_Textures[22], uv); break;
        case 23: texColor *= texture(u_Textures[23], uv); break;
        case 24: texColor *= texture(u_Textures[24], uv); break;
        case 25: texColor *= texture(u_Textures[25], uv); break;
        case 26: texColor *= texture(u_Textures[26], uv); break;
        case 27: texColor *= texture(u_Textures[27], uv); break;
        case 28: texColor *= texture(u_Textures[28], uv); break;
        case 29: texColor *= texture(u_Textures[29], uv); break;
        case 30: texColor *= texture(u_Textures[30], uv); break;
        case 31: texColor *= texture(u_Textures[31], uv); break;
    }
    if (texColor.a == 0.0)
    {
    	discard;
    }
    
    float dist = 1.0 - length(Input.LocalPosition.xy);
    float circleAlpha = smoothstep(0.0, Input.Fade, dist) * 
    					texColor.a * 
						smoothstep(Input.Thickness + Input.Fade, Input.Thickness, dist);
    
    if (circleAlpha == 0.0)
    {
    	discard;	
    }
    
	color = texColor;
	color.a *= circleAlpha;
	pickingID = v_EntityID;
}
