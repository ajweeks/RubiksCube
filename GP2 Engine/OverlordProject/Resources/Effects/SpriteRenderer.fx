
float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending 
{     
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
	DepthEnable = FALSE;
};

RasterizerState BackCulling 
{ 
	CullMode = BACK; 
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	uint TextureId: TEXCOORD0;
	float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
	float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
	float4 Color: COLOR;	
};

struct GS_DATA
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, 
	float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{
	pos.xy += offset;
	if (rotation != 0)
	{
		//Step 3.
		//Do rotation calculations
		//Transform to origin
		float2 origPos = pos.xy - pivotOffset;
		
		//Rotate
		pos.x = (origPos.x * rotCosSin.x) - (origPos.y * rotCosSin.y);
		pos.y = (origPos.y * rotCosSin.x) + (origPos.x * rotCosSin.y);
		
		//Retransform to initial position
		pos.xy += pivotOffset;
	}
	else
	{
		//Step 2.
		//No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
		//Just apply the pivot offset
		pos.xy -= pivotOffset;
	}

	//Geometry Vertex Output
	GS_DATA geomData = (GS_DATA) 0;
	geomData.Position = mul(float4(pos, 1.0f), gTransform);
	geomData.Color = col;
	geomData.TexCoord = texCoord;
	triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//Given Data (Vertex Data)
	float3 position = vertex[0].TransformData.xyz;
	float2 pivot = vertex[0].TransformData2.xy; // [0, 1]
	float rotation = vertex[0].TransformData.w;
	float2 scale = vertex[0].TransformData2.zw;
	float4 color = vertex[0].Color;

	float2 rotCosSin = float2(0, 0);
	if (rotation != 0)
	{
		rotCosSin = float2(cos(rotation), sin(rotation));
	}
	float2 scaledTextureSize = gTextureSize * scale;
	float2 offset = pivot * scaledTextureSize;
	pivot *= scaledTextureSize;

	// TL----------TR //TringleStrip (TL > TR > BL, BL > BR > TR)
	// |          / |
	// |       /    |
	// |    /       |
	// | /          |
	// BL----------BR
	
	//VERTEX 1 [TL]
	CreateVertex(triStream, position, color, float2(0, 0), rotation, rotCosSin, float2(0, 0), pivot); //Change the color data too!

	//VERTEX 2 [TR]
	CreateVertex(triStream, position, color, float2(1, 0), rotation, rotCosSin, float2(scaledTextureSize.x, 0), pivot); //Change the color data too!

	//VERTEX 3 [BL]
	CreateVertex(triStream, position, color, float2(0, 1), rotation, rotCosSin, float2(0, scaledTextureSize.y), pivot); //Change the color data too!

	//VERTEX 4 [BR]
	CreateVertex(triStream, position, color, float2(1, 1), rotation, rotCosSin, scaledTextureSize, pivot); //Change the color data too!
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET {

	return gSpriteTexture.Sample(samPoint, input.TexCoord) * input.Color;	
}

// Default Technique
technique10 Default {

	pass p0 {
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending,float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		//SetDepthStencilState(NoDepth,0);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
