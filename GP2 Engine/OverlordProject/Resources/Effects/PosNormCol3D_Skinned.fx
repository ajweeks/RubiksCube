float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float4x4 gBones[70];

struct VS_INPUT{
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHTS;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input) {
	VS_OUTPUT output;

	float4 originalPosition = float4(input.pos, 1.0);
	float4 transformedPosition = 0;
	float3 transformedNormal = 0;

	for (int i = 0; i < 4; ++i)
	{
		int boneIndex = input.blendIndices[i];

		if (boneIndex > -1)
		{
			transformedPosition += mul(originalPosition, gBones[boneIndex]) * input.blendWeights[i];
			transformedNormal += mul(input.normal, (float3x3)gBones[boneIndex]) * input.blendWeights[i];
		}
	}

	output.pos = mul(float4(transformedPosition.xyz, 1.0), gWorldViewProj); 
	output.normal = normalize(mul(transformedNormal, (float3x3)gWorld)); 

	output.color = input.color;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET{

	float3 color_rgb= input.color.rgb;
	float color_a = input.color.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;
	
	return float4( color_rgb , color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

technique11 TransparencyTech
{
	pass P0
	{
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}

