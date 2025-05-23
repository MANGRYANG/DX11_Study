struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 Normal : NORMAL;
    float4 color : COLOR;
};

float4 PS_Main(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}