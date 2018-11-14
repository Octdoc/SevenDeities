cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix cameraMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};


PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1;
	output.position = mul(input.position, worldMatrix);
	output.pos = output.position.xyz;
	output.position = mul(output.position, cameraMatrix);
	output.tex = input.tex;
	output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));
	output.tangent = normalize(mul(input.tangent, (float3x3)worldMatrix));
	output.binormal = normalize(mul(input.binormal, (float3x3)worldMatrix));

	return output;
}