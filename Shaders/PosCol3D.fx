
Texture2D gDiffuseMap : DIFFUSEMAP;
float  gAlphaRef : ALPHAREF = 0.5f;    // Global alpha ref for alpha testing

SamplerState samGeneral
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Mirror;
	AddressV = Mirror;
};

cbuffer cbPerObject
{
	float4x4 gWorldViewProj; 
};

struct VS_UV_INPUT
{
	float3 LocalPosition : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_INPUT
{
	float3 LocalPosition : POSITION;
	float4 Color : COLOR;
};

struct VS_UV_OUTPUT
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

struct PS_OUTPUT 
{
	float4 Color : SV_TARGET;
};

BlendState AlphaBlend
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

VS_UV_OUTPUT VS_UV(VS_UV_INPUT input)
{
	VS_UV_OUTPUT output;

	output.Position = mul(float4(input.LocalPosition, 1.0f), gWorldViewProj);
	output.TexCoord = input.TexCoord;

	return output;
}

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	output.Position = mul(float4(input.LocalPosition, 1.0f), gWorldViewProj);
	output.Color = input.Color;

	return output;
}

PS_OUTPUT PS(VS_OUTPUT input) 
{
	PS_OUTPUT output;

	output.Color = input.Color;

	return output;
}

PS_OUTPUT PS_UV(VS_UV_OUTPUT input)
{
	PS_OUTPUT output;

	float4 texColor = gDiffuseMap.Sample(samGeneral, input.TexCoord);
	
	float a = texColor.a;
	clip(a - gAlphaRef);
	
	output.Color = texColor;

	return output;
}

technique11 PosCol3DTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}

technique11 PosUVTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS_UV()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS_UV()));
		SetBlendState(AlphaBlend, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	}
};