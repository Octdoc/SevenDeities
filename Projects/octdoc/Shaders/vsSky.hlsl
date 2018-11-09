cbuffer MatrixBuffer
{
	matrix skyMatrix;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float3 tex : POSITION;
};

VertexOutput main(float4 position : POSITION)
{
	VertexOutput output;

	position.w = 1.0f;
	output.position = mul(position, skyMatrix).xyww;

	output.tex = position.xyz;

	return output;
}