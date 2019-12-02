cbuffer Cbuffer0
{
	matrix wvp;
	float4 color;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 pcolor : COLOR;
	float2 texcoord : TEXCOORD;

};

VOut PVS(float4 position : POSITION)
{
	VOut output;
	output.position = mul(wvp, position);
	output.pcolor = color;
	output.texcoord = position.xy;

	return output;

}

float4 PPS(float4 position : SV_POSITION, float4 pcolor : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{

	float distsq = (texcoord.x * texcoord.x) + (texcoord.y * texcoord.y);
	clip(1.f - distsq);
	return pcolor;

}