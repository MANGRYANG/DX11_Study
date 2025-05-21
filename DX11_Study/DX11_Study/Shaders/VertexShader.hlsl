struct VS_INPUT
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    // 동차 좌표계로 변환
    output.Pos = float4(input.Pos, 1.0f);
    output.Normal = input.Normal;
    output.Color = input.Color;
    return output;
}