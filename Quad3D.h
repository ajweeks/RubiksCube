#pragma once

#include "Shape.h"

class Quad3D : public Shape
{
public:
	Quad3D(ID3D11Device* pD3DDevice);
	virtual ~Quad3D();

	HRESULT Create(const wchar_t* texFilename, DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT3 p3, DirectX::XMFLOAT3 p4, float uMin, float uMax, float vMin, float vMax);
	HRESULT Create(const wchar_t* texFilename, DirectX::XMFLOAT3 pos, float width, float height, DirectX::XMFLOAT2 uMinMax = { 0, 1 }, DirectX::XMFLOAT2 vMinMax = { 0, 1 });
	HRESULT Create(const wchar_t* texFilename, float x, float y, float z, float width, float height, DirectX::XMFLOAT2 uMinMax = { 0, 1 }, DirectX::XMFLOAT2 vMinMax = { 0, 1 });

	void* operator new(size_t i);
	void operator delete(void* p);

	virtual void Paint(ID3D11DeviceContext* pDeviceContext) override;
	virtual void Update() override;

private:
	ID3DX11EffectShaderResourceVariable *m_pDiffuseMapVariable;
	ID3D11ShaderResourceView* m_pTextureResourceView = nullptr;
	ID3D11Resource* m_pTexture = nullptr;

};
