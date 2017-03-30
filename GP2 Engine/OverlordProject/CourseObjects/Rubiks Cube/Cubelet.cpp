
#include "stdafx.h"

#include "Cubelet.h"

#include "Components\MeshDrawComponent.h"
#include "Components\TransformComponent.h"
#include "CourseObjects\Rubiks Cube\RubiksCube.h"

Cubelet::Cubelet(float width, float height, float depth, XMFLOAT4 colours[6]) :
	m_Width(width), m_Height(height), m_Depth(depth)
{
	for (size_t i = 0; i < 6; i++)
	{
		m_Colors[i] = colours[i];
	}
}

Cubelet::~Cubelet()
{
}

void Cubelet::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	MeshDrawComponent* mesh = new MeshDrawComponent(12);

	//FRONT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 0, -1), m_Colors[0]),
		VertexPosNormCol(XMFLOAT3(m_Width, 0.0f, 0.0f), XMFLOAT3(0, 0, -1), m_Colors[0]),
		VertexPosNormCol(XMFLOAT3(m_Width, -m_Height, 0.0f), XMFLOAT3(0, 0, -1), m_Colors[0]),
		VertexPosNormCol(XMFLOAT3(0.0f, -m_Height, 0.0f), XMFLOAT3(0, 0, -1), m_Colors[0])
	);

	//BACK
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(m_Width, 0.0f, m_Depth), XMFLOAT3(0, 0, 1), m_Colors[2]),
		VertexPosNormCol(XMFLOAT3(0.0f, 0.0f, m_Depth), XMFLOAT3(0, 0, 1), m_Colors[2]),
		VertexPosNormCol(XMFLOAT3(0.0f, -m_Height, m_Depth), XMFLOAT3(0, 0, 1), m_Colors[2]),
		VertexPosNormCol(XMFLOAT3(m_Width, -m_Height, m_Depth), XMFLOAT3(0, 0, 1), m_Colors[2])
	);
	
	//LEFT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(0.0f, 0.0f, m_Depth), XMFLOAT3(-1, 0, 0), m_Colors[5]),
		VertexPosNormCol(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(-1, 0, 0), m_Colors[5]),
		VertexPosNormCol(XMFLOAT3(0.0f, -m_Height, 0.0f), XMFLOAT3(-1, 0, 0), m_Colors[5]),
		VertexPosNormCol(XMFLOAT3(0.0f, -m_Height, m_Depth), XMFLOAT3(-1, 0, 0), m_Colors[5])
	);
	
	//RIGHT
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(m_Width, 0.0f, 0.0f), XMFLOAT3(1, 0, 0), m_Colors[4]),
		VertexPosNormCol(XMFLOAT3(m_Width, 0.0f, m_Depth), XMFLOAT3(1, 0, 0), m_Colors[4]),
		VertexPosNormCol(XMFLOAT3(m_Width, -m_Height, m_Depth), XMFLOAT3(1, 0, 0), m_Colors[4]),
		VertexPosNormCol(XMFLOAT3(m_Width, -m_Height, 0.0f), XMFLOAT3(1, 0, 0), m_Colors[4])
	);
	
	//TOP
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(0.0f, 0.0f, m_Depth), XMFLOAT3(0, 1, 0), m_Colors[1]),
		VertexPosNormCol(XMFLOAT3(m_Width, 0.0f, m_Depth), XMFLOAT3(0, 1, 0), m_Colors[1]),
		VertexPosNormCol(XMFLOAT3(m_Width, 0.0f, 0.0f), XMFLOAT3(0, 1, 0), m_Colors[1]),
		VertexPosNormCol(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0, 1, 0), m_Colors[1])
	);
	
	//BOTTOM
	mesh->AddQuad(
		VertexPosNormCol(XMFLOAT3(0.0f, -m_Height, 0.0f), XMFLOAT3(0, -1, 0), m_Colors[3]),
		VertexPosNormCol(XMFLOAT3(m_Width, -m_Height, 0.0f), XMFLOAT3(0, -1, 0), m_Colors[3]),
		VertexPosNormCol(XMFLOAT3(m_Width, -m_Height, m_Depth), XMFLOAT3(0, -1, 0), m_Colors[3]),
		VertexPosNormCol(XMFLOAT3(0.0f, -m_Height, m_Depth), XMFLOAT3(0, -1, 0), m_Colors[3])
	);

	AddComponent(mesh);
}

void Cubelet::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void Cubelet::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
