#pragma once

#include "D3DUtil.h"

#include <tchar.h>
#include <vector>

#define ArraySize(a) (sizeof(a) / sizeof(a[0]))

struct VertexPosUV
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
};

struct VertexPosCol
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Color;
};

class Shape
{
public:
	Shape(UINT numVerts, UINT numIndicies, ID3D11Device* pD3DDevice, ID3DX11Effect* effect, ID3DX11EffectTechnique* technique, 
		ID3DX11EffectMatrixVariable* matWorldViewProjVariable);
	virtual ~Shape();
	virtual void Paint(ID3D11DeviceContext* pDeviceContext) = 0;
	virtual void Update() = 0;

	static void BuildAllEffects(ID3D11Device* D3DDevice);
	static void ReleaseAllEffects();

	void SetPos(DirectX::XMFLOAT3 position);
	void Move(DirectX::XMFLOAT3 position);
	DirectX::XMFLOAT3 GetPosition() const;

	void SetScale(DirectX::XMFLOAT3 scale);

	void Rotate(DirectX::XMFLOAT3 offset, DirectX::XMFLOAT3 deltaRot);

	void SetViewMatrix(const DirectX::XMFLOAT4X4& view);
	void SetProjectionMatrix(const DirectX::XMFLOAT4X4& projection);

	void ClearToRotate();

protected:
	void Paint(ID3D11DeviceContext* pDeviceContext, D3D_PRIMITIVE_TOPOLOGY topology);

	HRESULT CreateVertexBuffer(const char* techniqueName, void* vertexData, UINT ByteWidth, D3D11_INPUT_ELEMENT_DESC vertexDesc[], UINT numElements);
	HRESULT CreateVertexBuffer(const char* techniqueName, VertexPosCol vertices[]);
	HRESULT CreateVertexBuffer(const char* techniqueName, VertexPosUV vertices[]);
	HRESULT CreateIndexBuffer(DWORD indicies[]);

	DirectX::XMFLOAT3 m_Position;
	DirectX::XMMATRIX m_Rotation;
	DirectX::XMFLOAT3 m_Scale;
	DirectX::XMFLOAT4X4 m_View4x4, m_Projection4x4;

	ID3D11Device* m_pD3DDevice;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11InputLayout* m_pVertexLayout;
	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	ID3DX11EffectMatrixVariable *m_pMatWorldViewProjVariable;

	static ID3DX11Effect *s_pPosColEffect;
	static ID3DX11EffectTechnique *s_pPosColTechnique;
	static ID3DX11EffectTechnique *s_pPosUVTechnique;
	static ID3DX11EffectMatrixVariable *s_pPosColMatWorldViewProjVariable;

	const UINT m_NumVertices;
	const UINT m_NumIndicies;
	UINT m_VertexSizeInBytes;

private:
	Shape& operator=(const Shape&) = delete;
	Shape(const Shape&) = delete;

	static void BuildEffect(const std::wstring& filePath, const std::string& techniqueName, ID3DX11Effect** effect, 
		ID3DX11EffectTechnique** technique, ID3DX11EffectMatrixVariable** effectMatrixVariable, ID3D11Device* D3DDevice);

	bool m_UsesIndexBuffer;

};

DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3&rhs);
DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3&rhs);
