
#include "Shape.h"

ID3DX11Effect* Shape::s_pPosColEffect = nullptr;
ID3DX11EffectTechnique* Shape::s_pPosColTechnique = nullptr;
ID3DX11EffectTechnique* Shape::s_pPosUVTechnique = nullptr;
ID3DX11EffectMatrixVariable *Shape::s_pPosColMatWorldViewProjVariable = nullptr;

Shape::Shape(UINT numVerts, UINT numIndicies, ID3D11Device* pD3DDevice, ID3DX11Effect* effect, ID3DX11EffectTechnique* technique, ID3DX11EffectMatrixVariable* matWorldViewProjVariable) :
	m_pD3DDevice(pD3DDevice),
	m_pEffect(effect),
	m_pTechnique(technique),
	m_pMatWorldViewProjVariable(matWorldViewProjVariable),
	m_pVertexLayout(nullptr),
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_Rotation(XMMatrixIdentity()),
	m_Position(0.0f, 0.0f, 0.0f),
	m_Scale(1.0f, 1.0f, 1.0f),
	m_NumVertices(numVerts),
	m_NumIndicies(numIndicies),
	m_UsesIndexBuffer(false),
	m_VertexSizeInBytes(sizeof(float) * 3) // This should be overriden with the correct value, but we know there will be at least 3 floats in the buffer
{
}

Shape::~Shape()
{
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pMatWorldViewProjVariable) m_pMatWorldViewProjVariable->Release();
}

HRESULT Shape::CreateIndexBuffer(DWORD indicies[])
{
	m_UsesIndexBuffer = true;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_NumIndicies;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = indicies;
	HRESULT hr = m_pD3DDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("Shape couldn't be initialized!"), _T("Shape::CreateBufferIndexed unsuccessful"), MB_ICONERROR);
		exit(-1);
	}

	return hr;
}

HRESULT Shape::CreateVertexBuffer(const char* techniqueName, VertexPosUV vertices[])
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	const UINT numElements = ArraySize(vertexDesc);

	m_VertexSizeInBytes = sizeof(VertexPosUV);

	return CreateVertexBuffer(techniqueName, vertices, m_VertexSizeInBytes * m_NumVertices, vertexDesc, numElements);
}

HRESULT Shape::CreateVertexBuffer(const char* techniqueName, VertexPosCol vertices[])
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	const UINT numElements = ArraySize(vertexDesc);

	m_VertexSizeInBytes = sizeof(VertexPosCol);

	return CreateVertexBuffer(techniqueName, vertices, m_VertexSizeInBytes * m_NumVertices, vertexDesc, numElements);
}

HRESULT Shape::CreateVertexBuffer(const char* techniqueName, void* vertexData, UINT ByteWidth, D3D11_INPUT_ELEMENT_DESC vertexDesc[], UINT numElements)
{
	m_pTechnique = m_pEffect->GetTechniqueByName(techniqueName);
	
	D3DX11_PASS_DESC passDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = m_pD3DDevice->CreateInputLayout(vertexDesc, numElements,
		passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pVertexLayout);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = ByteWidth;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertexData;
	hr = m_pD3DDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("Shape couldn't be initialized!"), _T("Shape::CreateBuffer unsuccessful"), MB_ICONERROR);
		exit(-1);
	}

	return hr;
}

void Shape::BuildAllEffects(ID3D11Device* D3DDevice)
{
	BuildEffect(L"Shaders/PosCol3D.fx", "PosUVTech", &s_pPosColEffect, &s_pPosUVTechnique, &s_pPosColMatWorldViewProjVariable, D3DDevice);
	BuildEffect(L"Shaders/PosCol3D.fx", "PosCol3DTech", &s_pPosColEffect, &s_pPosColTechnique, &s_pPosColMatWorldViewProjVariable, D3DDevice);
}

void Shape::ReleaseAllEffects()
{
	if (s_pPosColEffect) s_pPosColEffect->Release();
}

void Shape::BuildEffect(const std::wstring& filePath, const std::string& techniqueName, 
	ID3DX11Effect** effect, ID3DX11EffectTechnique** technique, ID3DX11EffectMatrixVariable** effectMatrixVariable, ID3D11Device* D3DDevice)
{
	//http://msdn.microsoft.com/en-us/library/windows/desktop/gg615083(v=vs.85).aspx
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileEffectFromFile(filePath.c_str(), 0, 0, shaderFlags, 0, D3DDevice, effect, &compilationMsgs);

	if (compilationMsgs != 0)
	{
		OutputDebugStringA((LPCSTR)compilationMsgs->GetBufferPointer());
		if (compilationMsgs) compilationMsgs->Release();
	}

	if (FAILED(hr))
	{
		DXTRACE_ERR_MSGBOX(L"D3DX11CompileEffectFromFile", hr);
		exit(-1);
	}

	*technique = (*effect)->GetTechniqueByName(techniqueName.c_str());
	*effectMatrixVariable = (*effect)->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Shape::Paint(ID3D11DeviceContext* pDeviceContext, D3D_PRIMITIVE_TOPOLOGY topology)
{
	//Calculate the WVP matrix
	XMMATRIX matScaling = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	XMMATRIX matTranslation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMMATRIX matWorld = matScaling * m_Rotation * matTranslation;

	XMMATRIX matView = XMLoadFloat4x4(&m_View4x4);
	XMMATRIX matProjection = XMLoadFloat4x4(&m_Projection4x4);
	XMMATRIX matWVP = matWorld * matView * matProjection;

	//Copy the matrix to the Shader
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&matWVP));

	UINT offset = 0;
	UINT stride = m_VertexSizeInBytes;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	if (m_UsesIndexBuffer) pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetInputLayout(m_pVertexLayout);
	pDeviceContext->IASetPrimitiveTopology(topology);

	D3DX11_TECHNIQUE_DESC techniqueDesc;
	m_pTechnique->GetDesc(&techniqueDesc);
	for (size_t i = 0; i < techniqueDesc.Passes; i++)
	{
		m_pTechnique->GetPassByIndex(i)->Apply(0, pDeviceContext);
		if (m_UsesIndexBuffer)
		{
			pDeviceContext->DrawIndexed(m_NumIndicies, 0, 0);
		}
		else
		{
			pDeviceContext->Draw(m_NumVertices, 0);
		}
	}
}

void Shape::SetPos(XMFLOAT3 position)
{
	m_Position = position;
}

void Shape::Move(XMFLOAT3 position)
{
	m_Position = m_Position + position;
}

XMFLOAT3 Shape::GetPosition() const
{
	return m_Position;
}

void Shape::SetScale(XMFLOAT3 scale)
{
	m_Scale = scale;
}

void Shape::Rotate(XMFLOAT3 offset, XMFLOAT3 deltaRot)
{
	XMVECTOR rotVec = XMLoadFloat3(&deltaRot);
	XMMATRIX matRot = XMMatrixRotationRollPitchYawFromVector(rotVec);
	XMMATRIX matOffset = XMMatrixTranslation(offset.x, offset.y, offset.z);
	XMMATRIX matOffsetInv = XMMatrixTranslation(-offset.x, -offset.y, -offset.z); 
	m_Rotation = m_Rotation * (matOffset * matRot * matOffsetInv);
}

//void Shape::SetRotation(XMFLOAT3 rot)
//{
//	m_Rotation = rot;
//}

//void Shape::RotateAbout(XMFLOAT3 axis, float angle)
//{
//	XMVECTOR axisVector = XMLoadFloat3(&axis);
//	m_ToRotate = XMMatrixRotationAxis(axisVector, angle);
//}

//XMFLOAT3 Shape::GetRotation() const
//{
//	return m_Rotation;
//}

void Shape::SetViewMatrix(const XMFLOAT4X4& view4x4)
{
	m_View4x4 = view4x4;
}

void Shape::SetProjectionMatrix(const XMFLOAT4X4& projection4x4)
{
	m_Projection4x4 = projection4x4;
}

void Shape::ClearToRotate()
{
	m_Rotation = XMMatrixIdentity();
	//m_LayerRotation = { 0, 0, 0};
	//m_ToRotate = XMMatrixIdentity();
}


XMFLOAT3 operator+(const XMFLOAT3& lhs, const XMFLOAT3&rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

XMFLOAT3 operator-(const XMFLOAT3& lhs, const XMFLOAT3&rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}