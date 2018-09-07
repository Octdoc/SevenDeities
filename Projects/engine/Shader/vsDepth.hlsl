cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix lightMatrix;
};


struct VertexInputType
{
	float4 position : POSITION;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXTURE0;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, lightMatrix);

	output.depthPosition = output.position;

	return output;
}