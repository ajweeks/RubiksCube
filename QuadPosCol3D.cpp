
#include "QuadPosCol3D.h"

#include <assert.h>

using namespace DirectX;

QuadPosCol3D::QuadPosCol3D(ID3D11Device* pD3DDevice)
	: Shape(4, 6, pD3DDevice, Shape::s_pPosColEffect, Shape::s_pPosColTechnique, Shape::s_pPosColMatWorldViewProjVariable),
	m_Angle(0.0f)
{
}

QuadPosCol3D::~QuadPosCol3D(void)
{
}

HRESULT QuadPosCol3D::Create(XMFLOAT3 p1, XMFLOAT4 c1, XMFLOAT3 p2, XMFLOAT4 c2, XMFLOAT3 p3, XMFLOAT4 c3, XMFLOAT3 p4, XMFLOAT4 c4)
{
	VertexPosCol vertices[] = { { p1, c1 }, { p2, c2 }, { p3, c3 }, { p4, c4 } };
	DWORD indicies[] = { 0, 1, 2, 0, 2, 3 };

	assert(ArraySize(vertices) == m_NumVertices);
	assert(ArraySize(indicies) == m_NumIndicies);

	CreateVertexBuffer("PosCol3DTech", vertices);
	return Shape::CreateIndexBuffer(indicies);
}

HRESULT QuadPosCol3D::Create(XMFLOAT3 pos, float width, float height, XMFLOAT4 c1, XMFLOAT4 c2, XMFLOAT4 c3, XMFLOAT4 c4)
{
	return Create(XMFLOAT3(pos.x, pos.y, pos.z), c1,
		XMFLOAT3(pos.x + width, pos.y, pos.z), c2,
		XMFLOAT3(pos.x + width, pos.y - height, pos.z), c3,
		XMFLOAT3(pos.x, pos.y - height, pos.z), c4);
}

HRESULT QuadPosCol3D::Create(float x, float y, float z, float width, float height, XMFLOAT4 c1, XMFLOAT4 c2, XMFLOAT4 c3, XMFLOAT4 c4)
{
	return Create(XMFLOAT3(x, y, z), c1,
		XMFLOAT3(x + width, y, z), c2,
		XMFLOAT3(x + width, y - height, z), c3,
		XMFLOAT3(x, y - height, z), c4);
}

void QuadPosCol3D::Paint(ID3D11DeviceContext* pDeviceContext)
{
	Shape::Paint(pDeviceContext, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void QuadPosCol3D::Update()
{
	m_Angle += 1.0f / 60.0f;
	//SetRotation(XMFLOAT3(sin(m_Angle * 0.75f), 0.0f, 0.0f));
}
