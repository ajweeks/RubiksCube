#pragma once

#include "Scenegraph\GameObject.h"

class RigidBodyComponent;
class ColliderComponent;

class Cubelet : public GameObject
{
public:
	// Colours are specified in order of: FRONT, TOP, BACK, BOTTOM, RIGHT, LEFT
	Cubelet(float width, float height, float depth, XMFLOAT4 colours[6]);
	~Cubelet();

	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;

private:
	XMFLOAT4 m_Colors[6];

	float m_Width;
	float m_Height;
	float m_Depth;
};