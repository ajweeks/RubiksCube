#pragma once

#include "Shape.h"

class QuadPosCol3D : public Shape
{
public:
	QuadPosCol3D(ID3D11Device* pD3DDevice);
	virtual ~QuadPosCol3D(void);
	HRESULT Create(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT4 c1, DirectX::XMFLOAT3 p2, DirectX::XMFLOAT4 c2, DirectX::XMFLOAT3 p3, DirectX::XMFLOAT4 c3, DirectX::XMFLOAT3 p4, DirectX::XMFLOAT4 c4);
	HRESULT Create(DirectX::XMFLOAT3 pos, float width, float height, DirectX::XMFLOAT4 c1, DirectX::XMFLOAT4 c2, DirectX::XMFLOAT4 c3, DirectX::XMFLOAT4 c4);
	HRESULT Create(float x, float y, float z, float width, float height, DirectX::XMFLOAT4 c1, DirectX::XMFLOAT4 c2, DirectX::XMFLOAT4 c3, DirectX::XMFLOAT4 c4);

	virtual void Paint(ID3D11DeviceContext* pDeviceContext) override;
	virtual void Update() override;

private:
	float m_Angle;

};
