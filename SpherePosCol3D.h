#pragma once

#include "Shape.h"

class SpherePosCol3D : public Shape
{
public:
	SpherePosCol3D(ID3D11Device* pD3DDevice, XMFLOAT3 position = {});
	virtual ~SpherePosCol3D();

	SpherePosCol3D(const SpherePosCol3D&) = delete;
	SpherePosCol3D& operator=(const SpherePosCol3D&) = delete;

	void* operator new(size_t i);
	void operator delete(void* p);

	HRESULT Create(XMFLOAT3 origin, float radius, XMFLOAT4 col1, XMFLOAT4 col2, XMFLOAT4 col3);
	HRESULT Create(float x, float y, float z, float radius, XMFLOAT4 col1, XMFLOAT4 col2, XMFLOAT4 col3);

	void Paint(ID3D11DeviceContext* pDeviceContext) override;
	void Update() override;

private:
	float m_Elapsed;

};
