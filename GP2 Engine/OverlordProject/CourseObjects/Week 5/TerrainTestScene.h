#pragma once

#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"

class SkyBoxPrefab;
class SpriteFont;
class SpherePrefab;

class TerrainTestScene : public GameScene
{
public:
	TerrainTestScene();
	virtual ~TerrainTestScene();

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	enum class InputActions
	{
		UP, DOWN, LEFT, RIGHT, JUMP
	};

	SpriteFont* m_pSpriteFont;
	SkyBoxPrefab* m_CubeMapCube;

	GameObject* m_TerrainObject = nullptr;
	SpherePrefab* m_pSphere = nullptr;

	TerrainTestScene(const TerrainTestScene &obj);
	TerrainTestScene& operator=(const TerrainTestScene& obj);
};
