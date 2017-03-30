#pragma once
#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"

class Material;

class ComponentTestScene: public GameScene
{
public:
	ComponentTestScene(void);
	virtual ~ComponentTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_FpsInterval;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ComponentTestScene( const ComponentTestScene &obj);
	ComponentTestScene& operator=( const ComponentTestScene& obj);
};

