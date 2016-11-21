#pragma once

#include "Shape.h"

class CubePosCol3D : public Shape
{
public:
	CubePosCol3D(ID3D11Device* pD3DDevice, XMFLOAT3 position = {});
	virtual ~CubePosCol3D();

	CubePosCol3D(const CubePosCol3D&);
	CubePosCol3D& operator=(const CubePosCol3D&) = delete;

	void* operator new(size_t i);
	void operator delete(void* p);

	HRESULT Create(XMFLOAT3 origin, float width, float height, float depth, XMFLOAT4 c0, 
		XMFLOAT4 c1, XMFLOAT4 c2, XMFLOAT4 c3, XMFLOAT4 c4, XMFLOAT4 c5);
	HRESULT Create(float x, float y, float z, float width, float depth, float height, 
		XMFLOAT4 c0, XMFLOAT4 c1, XMFLOAT4 c2, XMFLOAT4 c3, XMFLOAT4 c4, XMFLOAT4 c5);

	void Paint(ID3D11DeviceContext* pDeviceContext) override;
	void Update() override;

private:
	float m_Elapsed;

};
