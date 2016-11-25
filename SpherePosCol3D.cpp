
#include "SpherePosCol3D.h"

#include <assert.h>

using namespace DirectX;

SpherePosCol3D::SpherePosCol3D(ID3D11Device* pD3DDevice, XMFLOAT3 position) :
	Shape(12, 60, pD3DDevice, Shape::s_pPosColEffect, Shape::s_pPosColTechnique, Shape::s_pPosColMatWorldViewProjVariable),
	m_Elapsed(0.0f)
{
}

SpherePosCol3D::~SpherePosCol3D()
{
}

void* SpherePosCol3D::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void SpherePosCol3D::operator delete(void* p)
{
	_mm_free(p);
}

HRESULT SpherePosCol3D::Create(XMFLOAT3 origin, float radius, XMFLOAT4 col1, XMFLOAT4 col2, XMFLOAT4 col3)
{
	return Create(origin.x, origin.y, origin.z, radius, col1, col2, col3);
}

HRESULT SpherePosCol3D::Create(float x, float y, float z, float radius, XMFLOAT4 col1, XMFLOAT4 col2, XMFLOAT4 col3)
{
	const float t = (1.0f + sqrt(5.0f)) / 2.0f;
	VertexPosCol vertices[] = { 
		{ { x - 1, y + t, z }, col1 },
		{ { x + 1, y + t, z }, col1 },
		{ { x - 1, y - t, z }, col1 },
		{ { x + 1, y - t, z }, col1 },

		{ { x, y - 1, z + t }, col2 },
		{ { x, y + 1, z + t }, col2 },
		{ { x, y - 1, z - t }, col2 },
		{ { x, y + 1, z - t }, col2 },

		{ { x + t, y, z - 1 }, col3 },
		{ { x + t, y, z + 1 }, col3 },
		{ { x - t, y, z - 1 }, col3 },
		{ { x - t, y, z + 1 }, col3 }
	};
	assert(ArraySize(vertices) == m_NumVertices);

	for (size_t i = 0; i < ArraySize(vertices); ++i)
	{
		vertices[i].Position = { vertices[i].Position.x * radius, vertices[i].Position.y * radius, vertices[i].Position.z * radius };
	}

	DWORD indicies[] = {
		0, 11, 5,
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,

		1, 5, 9,
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,

		3, 9, 4,
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,

		4, 9, 5, 
		2, 4, 11, 
		6, 2, 10,
		8, 6, 7,
		9, 8, 1
	};
	assert(ArraySize(indicies) == m_NumIndicies);

	// TODO: Add get middle point to refine shape

	if (FAILED(CreateVertexBuffer("PosCol3DTech", vertices)))
	{
		MessageBox(NULL, _T("SpherePosCol3D vertex buffer couldn't be created!"), _T("Whoops"), MB_ICONERROR);
		exit(-1);
	}
	return Shape::CreateIndexBuffer(indicies);
}

void SpherePosCol3D::Update()
{
}

void SpherePosCol3D::Paint(ID3D11DeviceContext* pDeviceContext)
{
	Shape::Paint(pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
