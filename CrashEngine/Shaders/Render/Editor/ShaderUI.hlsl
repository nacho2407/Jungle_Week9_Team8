#include "../../Common/VertexLayouts.hlsl"
#include "../../Resources/ConstantBuffers.hlsl"

Texture2D DiffuseTex : register(t0);
SamplerState LinearClamp : register(s0);

PS_Input_Tex VS_UI(VS_Input_PNCT input)
{
    PS_Input_Tex output;
    
    output.position = mul(float4(input.position, 1.0f), Model);
    output.texcoord = input.texcoord;
    
    return output;
}

float4 PS_UI(PS_Input_Tex input) : SV_TARGET
{
    float4 color = DiffuseTex.Sample(LinearClamp, input.texcoord);
    
    color *= PrimitiveColor;
    
    if (color.a < 0.05f)
        discard;
    return color;
}