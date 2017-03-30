//************
// VARIABLES *
//************
cbuffer cbPerObject
{
	float4x4 m_MatrixWorldViewProj : WORLDVIEWPROJECTION;
	float4x4 m_MatrixWorld : WORLD;
	float3 m_LightDir={0.2f,-1.0f,0.2f};
}

RasterizerState FrontCulling 
{ 
	CullMode = NONE; 
};

SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;// of Mirror of Clamp of Border
    AddressV = Wrap;// of Mirror of Clamp of Border
};

Texture2D m_TextureDiffuse;

//**********
// STRUCTS *
//**********
struct VS_DATA
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXCOORD0;
};

//****************
// VERTEX SHADER *
//****************
VS_DATA MainVS(VS_DATA vsData)
{
	return vsData;
}

//******************
// GEOMETRY SHADER *
//******************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float3 normal, float2 texCoord)
{
	//Step 1. Create a GS_DATA object
	GS_DATA result = (GS_DATA)0;
	
	//Step 2. Transform the position using the WVP Matrix and assign it to (GS_DATA object).Position (Keep in mind: float3 -> float4)
	result.Position = mul(float4(pos, 1.0), m_MatrixWorldViewProj),

	//Step 3. Transform the normal using the World Matrix and assign it to (GS_DATA object).Normal (Only Rotation, No translation!)
	result.Normal = mul(normal, (float3x3)m_MatrixWorld);

	//Step 4. Assign texCoord to (GS_DATA object).TexCoord
	result.TexCoord = texCoord;

	//Step 5. Append (GS_DATA object) to the TriangleStream parameter (TriangleStream::Append(...))
	triStream.Append(result);
}

[maxvertexcount(12)]
void SpikeGenerator(triangle VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{
	float spikeHeight = 8;

	float3 basePoint, top, left, right, front, spikeNormal1, spikeNormal2, spikeNormal3;

	//Step 1. Calculate The basePoint
	basePoint = (vertices[0].Position + vertices[1].Position + vertices[2].Position) / 3.0f;

	//Step 2. Calculate The normal of the basePoint
	float3 normal = (vertices[0].Normal + vertices[1].Normal + vertices[2].Normal) / 3.0f;

	//Step 3. Calculate The Spike's Top Position
	top = basePoint + spikeHeight * normal;
	
	//Step 4. Calculate The Left And Right Positions
	float edgeScale = 0.1;
	float3 scaledEdge1 = (vertices[2].Position - vertices[0].Position) * edgeScale;
	float3 scaledEdge2 = normalize(cross(scaledEdge1, normal)) * 0.4;
	left = basePoint + scaledEdge1 - scaledEdge2;
	right = basePoint - scaledEdge1 - scaledEdge2;
	front = basePoint + scaledEdge2;

	//Step 5. Calculate The Normal of the spike
	float3 spikeEdge1 = left - top;
	float3 spikeEdge2 = right - top;
	float3 spikeEdge3 = front - top;
	spikeNormal1 = normalize(cross(spikeEdge1, spikeEdge2));
	spikeNormal2 = normalize(cross(spikeEdge2, spikeEdge3));
	spikeNormal3 = normalize(cross(spikeEdge3, spikeEdge2));

	//Step 6. Create The Vertices [Complete code in CreateVertex(...)]

	//Create Existing Geometry
	CreateVertex(triStream, vertices[0].Position, vertices[0].Normal, vertices[0].TexCoord);
	CreateVertex(triStream, vertices[1].Position, vertices[1].Normal, vertices[1].TexCoord);
	CreateVertex(triStream, vertices[2].Position, vertices[2].Normal, vertices[2].TexCoord);

	//Restart the strip so we can add another (independent) triangle!
	triStream.RestartStrip();

	//Create Spike Geometry
	CreateVertex(triStream, top, spikeNormal1, float2(0,0));
	CreateVertex(triStream, left, spikeNormal1, float2(0,0));
	CreateVertex(triStream, right, spikeNormal1, float2(0, 0));

	triStream.RestartStrip();

	CreateVertex(triStream, top, spikeNormal2, float2(0, 0));
	CreateVertex(triStream, front, spikeNormal2, float2(0, 0));
	CreateVertex(triStream, right, spikeNormal2, float2(0, 0));

	triStream.RestartStrip();

	CreateVertex(triStream, top, spikeNormal3, float2(0, 0));
	CreateVertex(triStream, left, spikeNormal3, float2(0, 0));
	CreateVertex(triStream, front, spikeNormal3, float2(0, 0));
}

//***************
// PIXEL SHADER *
//***************
float4 MainPS(GS_DATA input) : SV_TARGET 
{
	input.Normal=-normalize(input.Normal);
	float alpha = m_TextureDiffuse.Sample(samLinear,input.TexCoord).a;
	float3 color = m_TextureDiffuse.Sample( samLinear,input.TexCoord ).rgb;
	float s = max(dot(m_LightDir,input.Normal), 0.4f);

	return float4(color*s,alpha);
}


//*************
// TECHNIQUES *
//*************
technique10 DefaultTechnique 
{
	pass p0 {
		SetRasterizerState(FrontCulling);	
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, SpikeGenerator()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}