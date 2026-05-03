#include "../../Common/VertexLayouts.hlsl"
#include "../../Resources/ConstantBuffers.hlsl"
#include "../../Resources/SystemSamplers.hlsl"

Texture2D FontAtlas : register(t0);

PS_Input_PCT VS_TextUI(VS_Input_PCT input)
{
    PS_Input_PCT output;
    
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    output.texcoord = input.texcoord;
    
    return output;
}

float4 PS_TextUI(PS_Input_PCT input) : SV_TARGET
{
    float4 sampledColor = FontAtlas.Sample(PointClampSampler, input.texcoord);
    float coverage = sampledColor.r;
    
    if (coverage < 0.1f)
        discard;
    
    return float4(input.color.rgb, input.color.a * coverage);
}