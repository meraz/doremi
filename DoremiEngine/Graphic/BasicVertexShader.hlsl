cbuffer CameraMatrixBuffer : register(b0)
{
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer ModelMatrixBuffer : register(b1)
{
    matrix worldMatrix;
};

struct VertexInputType
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

struct VOut
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal: NORMAL;
};

VOut main(VertexInputType input, uint instanceID : SV_InstanceID)
{
    VOut output;
    output.position = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);



    //output.position = float4(input.position,1.0f);
    output.normal = input.normal;
    output.texCoord = input.texCoord;
    return output;
}