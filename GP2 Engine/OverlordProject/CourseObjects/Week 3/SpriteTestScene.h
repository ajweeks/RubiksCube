#pragma once
#include "Scenegraph/GameScene.h"

class TextureData;

class SpriteTestScene : public GameScene
{
public:
	SpriteTestScene(void);
	virtual ~SpriteTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	float m_FpsInterval;
	TextureData* m_pTexture;
	GameObject* m_pObj;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteTestScene(const SpriteTestScene &obj);
	SpriteTestScene& operator=(const SpriteTestScene& obj);
};

