#pragma once

#include "BaseGame.h"

#include "D3DUtil.h"
#include <vector>
#include <queue>

class CubePosCol3D;
class SpherePosCol3D;
class Quad3D;

class RubiksCube final : public BaseGame
{
public:
	RubiksCube();
	virtual ~RubiksCube();

private:
	enum class RotationDirection
	{
		CW, CCW
	};
	enum class Layer
	{
		TOP, MIDDLE_Y, BOTTOM,
		LEFT, MIDDLE_X, RIGHT,
		FRONT, MIDDLE_Z, BACK
	};
	struct LayerRotationCommand
	{
		Layer m_RotatingLayer;
		RotationDirection m_RotatingLayerDirection;
	};

	static void SetRotsAndDirs();

	virtual HRESULT GameInitialize();
	virtual void GameUpdate();

	void SetWireframe(bool wireframe);
	void ToggleWireframe();
	void RotateActiveLayer();
	void RecalculateCubePointers();
	void AddLayerRotationCommand(LayerRotationCommand command);
	void AddLayerRotationCommand(Layer layer, RotationDirection rotationDirection);
	void RotateLayer(Layer layer, RotationDirection rotationDirection);

	void Scramble();
	void Reset();

	// 0 = z, 1 = x, 2 = y
	void RotateCube(int axis, RotationDirection direction);

	virtual void OnKeyDown(int vkCode) override;
	virtual void OnKeyUp(int vkCode) override;
	virtual void OnMouseWheel(int scrollAmount) override;

	static const size_t NUM_TRIANGLES = 2;
	static const size_t NUM_QUADS = 2;
	static const size_t NUM_CUBES = 27;
	static const size_t NUM_LINESTRIPS = 1;

	static const float CUBE_WIDTH;
	static const float SPACING;
	
	static XMFLOAT3 s_CubePositions[NUM_CUBES];

	CubePosCol3D *m_Cubes[NUM_CUBES];
	CubePosCol3D *m_BackdropCube;
	Quad3D* m_ControlsPlane;
	std::vector<SpherePosCol3D*> m_Spheres;
	ID3D11RasterizerState* m_RasterizerState = nullptr;

	float m_Elapsed = 0.0f;
	bool m_Wireframe = false;
	bool m_ShowCubeHelpers = false;

	std::queue<LayerRotationCommand> m_LayerRotationsToExecute; // This stores all rotations the player has input that haven't been completed yet, they will be executed in order
	bool m_IsRotatingLayer = false;
	size_t m_FramesOfLayerRotationRemaining;
	size_t m_FramesOfLayerRotation = 25;

	float m_CameraZ = -4.0f;

};
