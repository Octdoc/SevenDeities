TextureCube tex;
SamplerState ss;

struct VertexOutput
{
	float4 position : SV_POSITION;
	float3 tex : POSITION;
};

float4 main(VertexOutput input) : SV_TARGET
{
	return tex.Sample(ss, input.tex);
}