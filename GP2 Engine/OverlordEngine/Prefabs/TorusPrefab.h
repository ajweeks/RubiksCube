#pragma once
#include "..\Scenegraph\GameObject.h"

class TorusPrefab: public GameObject
{
public:
	TorusPrefab(float majorRadius = 1.f, UINT majorRadiusSteps = 10, float minorRadius = .2f, UINT minorRadiusSteps = 5, XMFLOAT4 color = (XMFLOAT4)Colors::Green);
	~TorusPrefab(void);

protected:

	virtual void Initialize(const GameContext& gameContext);

private:

	float m_MajorRadius, m_MinorRadius;
	UINT m_MajorRadiusSteps, m_MinorRadiusSteps;
	XMFLOAT4 m_Color;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TorusPrefab(const TorusPrefab& yRef);									
	TorusPrefab& operator=(const TorusPrefab& yRef);
};
