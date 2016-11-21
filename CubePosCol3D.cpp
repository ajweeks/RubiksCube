
#include "CubePosCol3D.h"

#include <assert.h>

CubePosCol3D::CubePosCol3D(ID3D11Device* pD3DDevice, XMFLOAT3 position) :
	Shape(36, 0, pD3DDevice, Shape::s_pPosColEffect, Shape::s_pPosColTechnique, Shape::s_pPosColMatWorldViewProjVariable),
	m_Elapsed(0.0f)
{
}

CubePosCol3D::~CubePosCol3D()
{
}

CubePosCol3D::CubePosCol3D(const CubePosCol3D& rhs) :
	Shape(36, 0, rhs.m_pD3DDevice, Shape::s_pPosColEffect, Shape::s_pPosColTechnique, Shape::s_pPosColMatWorldViewProjVariable)
{
	m_Elapsed = rhs.m_Elapsed;
	m_Position = rhs.m_Position;
	m_Rotation = rhs.m_Rotation;
}

void* CubePosCol3D::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void CubePosCol3D::operator delete(void* p)
{
	_mm_free(p);
}

void CubePosCol3D::Update()
{
}

HRESULT CubePosCol3D::Create(XMFLOAT3 origin, float width, float height, float depth,
	XMFLOAT4 c0, XMFLOAT4 c1, XMFLOAT4 c2, XMFLOAT4 c3, XMFLOAT4 c4, XMFLOAT4 c5)
{
	return Create(origin.x, origin.y, origin.z, width, height, depth, c0, c1, c2, c3, c4, c5);
}

HRESULT CubePosCol3D::Create(float x, float y, float z, float width, float height, float depth, 
	XMFLOAT4 c0, XMFLOAT4 c1, XMFLOAT4 c2, XMFLOAT4 c3, XMFLOAT4 c4, XMFLOAT4 c5)
{
	// NOTE: You can not use an index buffer if you want to specify a different colour (or texture) for each face
	//    4--------5
	//   /|       /|
	//  / |      / |
	// 0--------1  |
	// |  7-----|--6
	// | /      | / 
	// |/       |/  
	// 3--------2   

	XMFLOAT3 verts[] = {{ x, y, z },							// 0  front top left
						{ x + width, y, z },					// 1  front top right
						{ x + width, y - height, z },			// 2  front btm right
						{ x, y - height, z },					// 3  front btm left 
						{ x, y, z + depth },					// 4  back  top left
						{ x + width, y, z + depth },			// 5  back  top right
						{ x + width, y - height, z + depth },	// 6  back  btm right
						{ x, y - height, z + depth } };			// 7  back  btm left

	VertexPosCol vertices[] = { 
	{ verts[0], c0 }, // Front
	{ verts[1], c0 },
	{ verts[2], c0 },
	{ verts[0], c0 },
	{ verts[2], c0 },
	{ verts[3], c0 },

	{ verts[1], c1 }, // Right
	{ verts[5], c1 },
	{ verts[6], c1 },
	{ verts[1], c1 },
	{ verts[6], c1 },
	{ verts[2], c1 },

	{ verts[5], c2 }, // Back
	{ verts[4], c2 },
	{ verts[7], c2 },
	{ verts[5], c2 },
	{ verts[7], c2 },
	{ verts[6], c2 },

	{ verts[4], c3 }, // Left
	{ verts[0], c3 },
	{ verts[3], c3 },
	{ verts[4], c3 },
	{ verts[3], c3 },
	{ verts[7], c3 },

	{ verts[4], c4 }, // Top
	{ verts[5], c4 },
	{ verts[1], c4 },
	{ verts[4], c4 },
	{ verts[1], c4 },
	{ verts[0], c4 },

	{ verts[3], c5 }, // Back
	{ verts[2], c5 },
	{ verts[6], c5 },
	{ verts[3], c5 },
	{ verts[6], c5 },
	{ verts[7], c5 }};	

	assert(ArraySize(vertices) == m_NumVertices);

	// For blended vertex colors (or solid shapes) use an index buffer like:
	//DWORD indicies[] = {
	//	0, 1, 2, 0, 2, 3, // Front
	//	1, 5, 6, 1, 6, 2, // Right
	//	5, 4, 7, 5, 7, 6, // Back
	//	4, 0, 3, 4, 3, 7, // Left
	//	4, 5, 1, 4, 1, 0, // Top
	//	3, 2, 6, 3, 6, 7, // Bottom
	//};	
	//assert(ArraySize(indicies) == m_NumIndicies);

	return Shape::CreateVertexBuffer("PosCol3DTech", vertices);
}

void CubePosCol3D::Paint(ID3D11DeviceContext* pDeviceContext)
{
	Shape::Paint(pDeviceContext, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
