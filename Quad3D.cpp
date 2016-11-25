
#include "Quad3D.h"
#include "AdditionalLibraries\DX_Toolkit\include\DDSTextureLoader.h"

#include <assert.h>

using namespace DirectX;

Quad3D::Quad3D(ID3D11Device* pD3DDevice)
	: Shape(4, 6, pD3DDevice, Shape::s_pPosColEffect, Shape::s_pPosColTechnique, Shape::s_pPosColMatWorldViewProjVariable)
{
}

Quad3D::~Quad3D()
{
	if (m_pDiffuseMapVariable) m_pDiffuseMapVariable->Release();
}

void* Quad3D::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

void Quad3D::operator delete(void* p)
{
	_mm_free(p);
}

HRESULT Quad3D::Create(const wchar_t* texFilename, XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3, XMFLOAT3 p4, float uMin, float uMax, float vMin, float vMax)
{
	DDS_ALPHA_MODE alphaMode = DDS_ALPHA_MODE_PREMULTIPLIED;
	HRESULT hr = CreateDDSTextureFromFile(m_pD3DDevice, texFilename, &m_pTexture, &m_pTextureResourceView, 0, &alphaMode);
	if (FAILED(hr)) DXTRACE_ERR_MSGBOX(L"Create CreateDDSTextureFromFile Failed", hr);

	VertexPosUV vertices[] = { 
		{ p1, { uMin, vMin } },
		{ p2, { uMax, vMin } },
		{ p3, { uMax, vMax } },
		{ p4, { uMin, vMax } } };
	DWORD indicies[] = { 0, 1, 2, 0, 2, 3 };

	assert(ArraySize(vertices) == m_NumVertices);
	assert(ArraySize(indicies) == m_NumIndicies);

	m_pDiffuseMapVariable = m_pEffect->GetVariableBySemantic("DIFFUSEMAP")->AsShaderResource();
	if (!m_pDiffuseMapVariable->IsValid()) DXTRACE_ERR_MSGBOX(L"Semantic DIFFUSEMAP not valid", S_FALSE);

	
	if (FAILED(CreateVertexBuffer("PosUVTech", vertices)))
	{
		MessageBox(NULL, _T("Vertex buffer couldn't be initialized!"), _T("Shape::CreateBuffer unsuccessful"), MB_ICONERROR);
		exit(-1);
	}
	
	return Shape::CreateIndexBuffer(indicies);
}

HRESULT Quad3D::Create(const wchar_t* texFilename, XMFLOAT3 pos, float width, float height, XMFLOAT2 uMinMax, XMFLOAT2 vMinMax)
{
	return Create(texFilename, pos.x, pos.y, pos.z, width, height, uMinMax, vMinMax);
}

HRESULT Quad3D::Create(const wchar_t* texFilename, float x, float y, float z, float width, float height, XMFLOAT2 uMinMax, XMFLOAT2 vMinMax)
{
	return Create(texFilename, 
		XMFLOAT3(x, y, z),
		XMFLOAT3(x + width, y, z),
		XMFLOAT3(x + width, y - height, z),
		XMFLOAT3(x, y - height, z), 
		uMinMax.x, uMinMax.y, vMinMax.x, vMinMax.y);
}

void Quad3D::Paint(ID3D11DeviceContext* pDeviceContext)
{
	m_pDiffuseMapVariable->SetResource(m_pTextureResourceView);

	Shape::Paint(pDeviceContext, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Quad3D::Update()
{
}
