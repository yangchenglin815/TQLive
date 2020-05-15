struct VSInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};
struct PSInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};
//vertex shader
cbuffer MatrixBuffer
{
	matrix mvp;
};
PSInput VS(VSInput input)
{
	PSInput output;
	input.position.w = 1.0f;
	output.position = mul(input.position, mvp);
	output.tex = input.tex;
	return output;
}
//pixel shader
Texture2D u_texY;
Texture2D u_texU;
Texture2D u_texV;
SamplerState SampleType;
float4 PS(PSInput input) : SV_TARGET
{
	float y = u_texY.Sample(SampleType, input.tex).r;
	float u = u_texU.Sample(SampleType, input.tex).r  - 0.5f;
	float v = u_texV.Sample(SampleType, input.tex).r  - 0.5f;
	float r = y + 1.14f * v;
	float g = y - 0.394f * u - 0.581f * v;
	float b = y + 2.03f * u;
	return float4(r,g,b, 1.0f);
}
