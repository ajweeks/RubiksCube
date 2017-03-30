#pragma once
#include "..\Scenegraph\GameObject.h"

class SpherePrefab: public GameObject
{
public:
	SpherePrefab(float radius = 1.f, int steps = 10, XMFLOAT4 color = (XMFLOAT4)Colors::Blue);
	~SpherePrefab(void);

protected:

	virtual void Initialize(const GameContext& gameContext);

private:

	float m_Radius;
	int m_Steps;
	XMFLOAT4 m_Color;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpherePrefab(const SpherePrefab& yRef);									
	SpherePrefab& operator=(const SpherePrefab& yRef);
};
