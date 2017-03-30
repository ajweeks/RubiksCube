#pragma once

#include "Scenegraph\GameObject.h"

#include "Cubelet.h"

#include <vector>
#include <queue>

struct GameContext;

class RubiksCube : public GameObject
{
public:
	RubiksCube();
	~RubiksCube();

	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;

	static const float CUBE_WIDTH;
	static const float SPACING;
	static const float HOVER_SCALE;

private:
	enum class RotationDirection
	{
		CW, CCW,
		NONE
	};
	enum class Layer
	{
		TOP, MIDDLE_Y, BOTTOM,
		LEFT, MIDDLE_X, RIGHT,
		FRONT, MIDDLE_Z, BACK,
		NONE
	};
	struct LayerRotationCommand
	{
		Layer m_RotatingLayer;
		RotationDirection m_RotatingLayerDirection;
	};

	static void SetRotsAndDirs();

	void RotateActiveLayer();
	void RecalculateCubePointers();
	void AddLayerRotationCommand(LayerRotationCommand command);
	void AddLayerRotationCommand(Layer layer, RotationDirection rotationDirection);
	void RotateLayer(Layer layer, RotationDirection rotationDirection);
	std::vector<Cubelet*> GetLayerCubes(Layer layer);

	void CreateCubes();
	void Scramble();
	void Reset();

	// 0 = z, 1 = x, 2 = y
	void RotateCube(int axis, RotationDirection direction);

	static const size_t CUBE_INDICIES[9][9];

	static const size_t NUM_CUBES = 27;

	static XMFLOAT3 s_CubePositions[NUM_CUBES];

	Cubelet *m_Cubes[NUM_CUBES];
	GameObject* m_CubeColliders[NUM_CUBES];

	std::queue<LayerRotationCommand> m_LayerRotationsToExecute; // This stores all rotations the player has input that haven't been completed yet, they will be executed in order
	bool m_IsRotatingLayer = false;
	size_t m_FramesOfLayerRotationRemaining = 0;
	size_t m_FramesOfLayerRotation = 240;

	Layer m_HighlightedLayer;
};
