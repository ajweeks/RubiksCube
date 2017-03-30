#pragma once
#include "Scenegraph/GameScene.h"

class SpriteFont;

class SpriteFontTestScene : public GameScene
{
public:
	SpriteFontTestScene();
	virtual ~SpriteFontTestScene();

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	SpriteFont* m_pSpriteFont;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteFontTestScene(const SpriteFontTestScene &obj);
	SpriteFontTestScene& operator=(const SpriteFontTestScene& obj);
};

