#pragma once

#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"

class SpriteFont;
class RubiksCube;

class RubiksCubeScene : public GameScene
{
public:
	RubiksCubeScene();
	virtual ~RubiksCubeScene();

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	RubiksCube* m_RubiksCube;
	SpriteFont* m_pSpriteFont;

	RubiksCubeScene(const RubiksCubeScene &obj);
	RubiksCubeScene& operator=(const RubiksCubeScene& obj);
};
