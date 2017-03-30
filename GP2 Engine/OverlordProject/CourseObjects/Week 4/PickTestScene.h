#pragma once

#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"

class SpriteFont;

class PickTestScene : public GameScene
{
public:
	PickTestScene();
	virtual ~PickTestScene();

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	SpriteFont* m_pSpriteFont;

	PickTestScene(const PickTestScene &obj);
	PickTestScene& operator=(const PickTestScene& obj);
};
