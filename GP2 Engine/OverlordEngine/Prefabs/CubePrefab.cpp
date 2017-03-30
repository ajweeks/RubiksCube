//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CubePrefab.h"
#include "../Components/Components.h"


CubePrefab::CubePrefab(float width, float height, float depth, XMFLOAT4 color):
	 m_Width(width),
	 m_Height(height),
	 m_Depth(depth),
	 m_Color(color)
{
}


CubePrefab::~CubePrefab(void)
{
}

void CubePrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Create Cube
	auto mesh = new MeshDrawComponent(12);

	float halfWidth = m_Width/2.f;
	float halfHeight = m_Height/2.f;
	float halfDepth = m_Depth/2.f;

	//FRONT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, -halfDepth), XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, -halfDepth), XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, -halfDepth), XMFLOAT3(0, 0, -1), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), XMFLOAT3(0, 0, -1), m_Color)
		);

	//BACK
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT3(0, 0, 1), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, halfDepth), XMFLOAT3(0, 0, 1), m_Color),	
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, halfDepth), XMFLOAT3(0, 0, 1), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, halfDepth), XMFLOAT3(0, 0, 1), m_Color)	
		);

	//LEFT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, halfDepth), XMFLOAT3(-1, 0, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, -halfDepth), XMFLOAT3(-1, 0, 0), m_Color),	
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), XMFLOAT3(-1, 0, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, halfDepth), XMFLOAT3(-1, 0, 0), m_Color)	
		);

	//RIGHT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, -halfDepth), XMFLOAT3(1, 0, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT3(1, 0, 0), m_Color),	
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, halfDepth), XMFLOAT3(1, 0, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, -halfDepth), XMFLOAT3(1, 0, 0), m_Color)	
		);

	//TOP
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, halfDepth), XMFLOAT3(0, 1, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, halfDepth), XMFLOAT3(0, 1, 0), m_Color),	
		VertexPosNormCol(XMFLOAT3(halfWidth, halfHeight, -halfDepth), XMFLOAT3(0, 1, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, halfHeight, -halfDepth), XMFLOAT3(0, 1, 0), m_Color)	
		);

	//BOTTOM
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, -halfDepth), XMFLOAT3(0, -1, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, -halfDepth), XMFLOAT3(0, -1, 0), m_Color),	
		VertexPosNormCol(XMFLOAT3(halfWidth, -halfHeight, halfDepth), XMFLOAT3(0, -1, 0), m_Color),
		VertexPosNormCol(XMFLOAT3(-halfWidth, -halfHeight, halfDepth), XMFLOAT3(0, -1, 0), m_Color)	
		);

	AddComponent(mesh);
}