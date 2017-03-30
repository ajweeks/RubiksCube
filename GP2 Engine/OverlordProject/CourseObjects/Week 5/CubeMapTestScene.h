#pragma once

#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"

class SkyBoxPrefab;
class SpriteFont;

class CubeMapTestScene : public GameScene
{
public:
	CubeMapTestScene();
	virtual ~CubeMapTestScene();

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	SpriteFont* m_pSpriteFont;
	SkyBoxPrefab* m_CubeMapCube;

	CubeMapTestScene(const CubeMapTestScene &obj);
	CubeMapTestScene& operator=(const CubeMapTestScene& obj);
};
