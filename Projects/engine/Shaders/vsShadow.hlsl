cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix cameraMatrix;
	matrix lightMatrix;
};

cbuffer LightBuffer
{
	float3 lightPosition;
	float padding;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	float4 worldPosition;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.pos = output.position.xyz;
	output.position = mul(output.position, cameraMatrix);

	output.lightViewPosition = mul(input.position, worldMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, lightMatrix);

	output.tex = input.tex;

	output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));
	output.tangent = normalize(mul(input.tangent, (float3x3)worldMatrix));
	output.binormal = normalize(mul(input.binormal, (float3x3)worldMatrix));

	worldPosition = mul(input.position, worldMatrix);

	output.lightPos = lightPosition.xyz - worldPosition.xyz;

	output.lightPos = normalize(output.lightPos);

	return output;
}