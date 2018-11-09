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
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 pos : POSITION;
	float3 normal : NORMAL;
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

	output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));

	worldPosition = mul(input.position, worldMatrix);

	output.lightPos = lightPosition.xyz - worldPosition.xyz;

	output.lightPos = normalize(output.lightPos);

	return output;
}