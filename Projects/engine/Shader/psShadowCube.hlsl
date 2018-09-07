Texture2D textures[2];
TextureCube shadowMap;
SamplerState samplers[2];


cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightPosition;
	float ambient;
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

float4 main(PixelInputType input) : SV_TARGET
{
	float bias;
	float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 textureColor;

	bias = 0.00001f;

	color = ambientColor;

	depthValue = shadowMap.Sample(samplers[0], -input.pos - lightPosition).r;

	lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
	lightDepthValue = lightDepthValue - bias;

	if (lightDepthValue < depthValue)
	{
		lightIntensity = saturate(dot(input.normal, input.lightPos + input.pos));

		if (lightIntensity > 0.0f)
		{
			color += (diffuseColor * lightIntensity);
			color = saturate(color);
		}
	}

	textureColor = textures[0].Sample(samplers[1], input.tex);

	color = color * textureColor;

	return color;
}