#pragma once

#include "Scenegraph/GameScene.h"
#include "Helpers\EffectHelper.h"

class SkyBoxPrefab;
class SpriteFont;
class SpherePrefab;
class RigidBodyComponent;

class GeometryShaderTestScene : public GameScene
{
public:
	GeometryShaderTestScene();
	virtual ~GeometryShaderTestScene();

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	enum class InputActions
	{
		UP, DOWN, LEFT, RIGHT, 
		JUMP,
		RESET
	};

	void Reset();

	SpriteFont* m_pSpriteFont;
	SkyBoxPrefab* m_CubeMapCube;

	GameObject* m_TerrainObject = nullptr;
	SpherePrefab* m_pSphere = nullptr;
	RigidBodyComponent* m_pSphereRBComponent = nullptr;

	GeometryShaderTestScene(const GeometryShaderTestScene &obj);
	GeometryShaderTestScene& operator=(const GeometryShaderTestScene& obj);
};

/*

	Geometry shader notes:

	Possibilities:
		- Grass
		- Trees
		- Wet fur
		- Hair
		- Metaballs
		- Object re-entry
		- Flames
		- Feathers?
		- Geometric landscape (sine wave cubes)
		- 

*/