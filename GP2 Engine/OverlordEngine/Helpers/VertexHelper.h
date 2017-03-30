#pragma once

struct VertexPosCol
{
	public:

	VertexPosCol(){};
	VertexPosCol(XMFLOAT3 pos, XMFLOAT4 col):
		Position(pos), Color(col){}

	VertexPosCol(const VertexPosCol& rhs) :
		Position(rhs.Position),
		Color(rhs.Color)
	{
	}

	VertexPosCol(VertexPosCol&& rhs)
	{
		std::swap(Position, rhs.Position);
		std::swap(Color, rhs.Color);
	}

	VertexPosCol& operator=(const VertexPosCol& rhs)
	{
		Position = rhs.Position;
		Color = rhs.Color;

		return *this;
	}

	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

struct VertexPosNormCol
{
public:

	VertexPosNormCol(){};
	VertexPosNormCol(XMFLOAT3 pos, XMFLOAT3 norm, XMFLOAT4 col):
		Position(pos), Normal(norm), Color(col){}

	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Color;
};

struct QuadPosNormCol
{
public:

	QuadPosNormCol(){};
	QuadPosNormCol(VertexPosNormCol vert1, VertexPosNormCol vert2, VertexPosNormCol vert3, VertexPosNormCol vert4):
		Vertex1(vert1), Vertex2(vert2), Vertex3(vert3), Vertex4(vert4){}

	VertexPosNormCol Vertex1;
	VertexPosNormCol Vertex2;
	VertexPosNormCol Vertex3;
	VertexPosNormCol Vertex4;
};

struct TrianglePosNormCol
{
public:

	TrianglePosNormCol(){};
	TrianglePosNormCol(VertexPosNormCol vert1, VertexPosNormCol vert2, VertexPosNormCol vert3):
		Vertex1(vert1), Vertex2(vert2), Vertex3(vert3){}

	VertexPosNormCol Vertex1;
	VertexPosNormCol Vertex2;
	VertexPosNormCol Vertex3;
};

// UPDATE PP
struct VertexPosTex
{
	public:

	VertexPosTex(){};
	VertexPosTex(XMFLOAT3 pos, XMFLOAT2 uv):
		Position(pos), UV(uv){}

	XMFLOAT3 Position;
	XMFLOAT2 UV;
};

struct QuadPosTex
{
public:

	QuadPosTex(){};
	QuadPosTex(VertexPosTex vert1, VertexPosTex vert2, VertexPosTex vert3, VertexPosTex vert4):
		Vertex1(vert1), Vertex2(vert2), Vertex3(vert3), Vertex4(vert4){}

	VertexPosTex Vertex1;
	VertexPosTex Vertex2;
	VertexPosTex Vertex3;
	VertexPosTex Vertex4;
};

struct VertexPosNormTex
{
public:

	VertexPosNormTex() {};
	VertexPosNormTex(XMFLOAT3 pos, XMFLOAT3 norm, XMFLOAT2 texCoord) :
		Position(pos), Normal(norm), TexCoord(texCoord) {}

	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
};