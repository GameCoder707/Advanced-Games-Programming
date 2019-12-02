TextureCube cube0;
SamplerState sampler0;

cbuffer CB0
{
	matrix WVPMatrix; // 64 bytes

}; // 64 bytes

struct VOut
{
	float4 position : SV_POSITION;
	float3 texcoord : TEXCOORD;

};

VOut VS(float4 position : POSITION, float3 texcoord : TEXCOORD)
{
	VOut output;

	output.texcoord = position.xyz;
	output.position = mul(WVPMatrix, position);

	return output;
}

float4 PS(VOut input) : SV_TARGET
{
	return cube0.Sample(sampler0, input.texcoord);
}