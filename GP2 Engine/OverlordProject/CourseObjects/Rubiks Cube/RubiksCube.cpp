
#include "stdafx.h"

#include "RubiksCube.h"

#include "Components\TransformComponent.h"
#include "Base\GeneralStructs.h"
#include "Components\MeshDrawComponent.h"
#include "Physx\PhysxManager.h"

#include "Components\ColliderComponent.h"
#include "Components\RigidBodyComponent.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <time.h>

XMFLOAT3 RubiksCube::s_CubePositions[27];

const float RubiksCube::CUBE_WIDTH = 0.3f;
const float RubiksCube::SPACING = 0.05f;
const float RubiksCube::HOVER_SCALE = 1.05f;

const size_t RubiksCube::CUBE_INDICIES[9][9] = {
	{ 6, 7, 8, 15, 16, 17, 24, 25, 26 },	// Top
	{ 3, 4, 5, 12, 13, 14, 21, 22, 23 },	// Middle Y
	{ 0, 1, 2, 9, 10, 11, 18, 19, 20 },		// Bottom
	{ 0, 3, 6, 9, 12, 15, 18, 21, 24 },		// Left
	{ 1, 4, 7, 10, 13, 16, 19, 22, 25 },	// Middle X
	{ 2, 5, 8, 11, 14, 17, 20, 23, 26 },	// Right
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8 },			// Front
	{ 9, 10, 11, 12, 13, 14, 15, 16, 17 },	// Middle Z
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26 },	// Back
};

RubiksCube::RubiksCube() :
	GameObject(),
	m_HighlightedLayer(Layer::NONE)
{
	SetRotsAndDirs();

	for (size_t i = 0; i < NUM_CUBES; ++i)
	{
		m_Cubes[i] = nullptr;
	}

	srand((UINT)time(0));
}

RubiksCube::~RubiksCube()
{
}

void RubiksCube::CreateCubes()
{
	auto physx = PhysxManager::GetInstance()->GetPhysics();
	PxMaterial* defaultMaterial = physx->createMaterial(0.5f, 0.5f, 0.1f);

	const XMFLOAT4 RED = { 0.9f, 0.0f, 0.0f, 1.0f };
	const XMFLOAT4 GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
	const XMFLOAT4 BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
	const XMFLOAT4 YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
	const XMFLOAT4 ORANGE = { 0.9f, 0.4f, 0.0f, 1.0f };
	const XMFLOAT4 PINK = { 1.0f, 0.0f, 1.0f, 1.0f };
	const XMFLOAT4 CYAN = { 0.0f, 1.0f, 1.0f, 1.0f };
	const XMFLOAT4 WHITE = { 1.0f, 1.0f, 1.0f, 1.0f };
	const XMFLOAT4 BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
	const XMFLOAT4 OFF_BLACK = { 0.1f, 0.1f, 0.15f, 1.0f };

	for (size_t i = 0; i < NUM_CUBES; ++i)
	{
		const size_t cubeX = (i % 3);
		const size_t cubeY = ((i / 3) % 3);
		const size_t cubeZ = (i / 9);

		enum class Faces
		{
			FRONT, TOP, BACK, BOTTOM, RIGHT, LEFT
			//RED, GREEN, ORANGE, BLUE, WHITE, YELLOW
		};

		XMFLOAT4 faceColors[6];
		for (size_t j = 0; j < 6; ++j)
			faceColors[j] = OFF_BLACK;

		if (cubeY == 0)
			faceColors[int(Faces::BOTTOM)] = YELLOW;
		else if (cubeY == 2)
			faceColors[int(Faces::TOP)] = WHITE;

		if (cubeX == 0)
			faceColors[int(Faces::LEFT)] = BLUE;
		else if (cubeX == 2)
			faceColors[int(Faces::RIGHT)] = GREEN;

		if (cubeZ == 0)
			faceColors[int(Faces::FRONT)] = RED;
		else if (cubeZ == 2)
			faceColors[int(Faces::BACK)] = ORANGE;

		m_Cubes[i] = new Cubelet(CUBE_WIDTH, CUBE_WIDTH, CUBE_WIDTH, faceColors);
		m_Cubes[i]->GetTransform()->Translate(s_CubePositions[i]);
		m_Cubes[i]->GetTransform()->SetStoreRotationMatrix(true);

		AddChild(m_Cubes[i]);

		// Collider
		m_CubeColliders[i] = new GameObject();
		m_CubeColliders[i]->GetTransform()->Translate(s_CubePositions[i]);

		const float halfWidth = CUBE_WIDTH / 2.0f;
		const float halfHeight = CUBE_WIDTH / 2.0f;
		const float halfDepth = CUBE_WIDTH / 2.0f;
		std::shared_ptr<PxGeometry> cubeGeometry(new PxBoxGeometry(halfWidth, halfHeight, halfDepth));
		ColliderComponent* colliderComponent = new ColliderComponent(cubeGeometry, *defaultMaterial, PxTransform(PxVec3(halfWidth, -halfHeight, halfDepth)));
		m_CubeColliders[i]->AddComponent(colliderComponent);

		RigidBodyComponent* rigidBodyComponent = new RigidBodyComponent();
		rigidBodyComponent->SetKinematic(true);
		m_CubeColliders[i]->AddComponent(rigidBodyComponent);

		AddChild(m_CubeColliders[i]);
	}
}

void RubiksCube::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	CreateCubes();
}

void RubiksCube::SetRotsAndDirs()
{
	for (size_t i = 0; i < NUM_CUBES; ++i)
	{
		const float cubeX = (i % 3) * (CUBE_WIDTH + SPACING);
		const float cubeY = ((i / 3) % 3) * (CUBE_WIDTH + SPACING);
		const float cubeZ = (i / 9) * (CUBE_WIDTH + SPACING);

		s_CubePositions[i] = { cubeX, cubeY, cubeZ };
	}
}

void RubiksCube::Update(const GameContext& gameContext)
{
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		m_Cubes[i]->GetTransform()->Scale(1.0f, 1.0f, 1.0f);
	}
	GameObject* pickedObject = gameContext.pCamera->Pick(gameContext);
	if (pickedObject)
	{
		Layer layer = Layer::NONE;
		
		int layerX = -1;
		int layerY = -1;
		int layerZ = -1;

		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			if (pickedObject == m_CubeColliders[i])
			{
				layerX = (i % 3);
				layerY = ((i / 3) % 3);
				layerZ = (i / 9);
				break;
			}
		}

		if (layerX != -1)
		{
			if (layerX == 0)
			{
				layer = layerY == 2 ? Layer::TOP : layerY == 1 ? Layer::LEFT : Layer::BOTTOM;
			}
			else if (layerX == 1)
			{
				layer = layerY == 1 ? Layer::FRONT : Layer::MIDDLE_X;
			}
			else
			{
				layer = layerY == 2 ? Layer::TOP : layerY == 1 ? Layer::RIGHT : Layer::BOTTOM;
			}

			std::vector<Cubelet*> layerCubes = GetLayerCubes(layer);
			for (size_t i = 0; i < layerCubes.size(); i++)
			{
				layerCubes[i]->GetTransform()->Scale(HOVER_SCALE, HOVER_SCALE, HOVER_SCALE);
			}

			// Left click direction, followed by right click direction, for each layer
			RotationDirection rotationDirecions[] = 
			{
				RotationDirection::CW, RotationDirection::CCW, // Top
				RotationDirection::CW, RotationDirection::CCW, // Middle Y
				RotationDirection::CCW, RotationDirection::CW, // Bottom
				RotationDirection::CCW, RotationDirection::CW, // Left
				RotationDirection::CW, RotationDirection::CCW, // Middle X
				RotationDirection::CW, RotationDirection::CCW, // Right
				RotationDirection::CCW, RotationDirection::CW, // Front
				RotationDirection::CCW, RotationDirection::CW, // Middle Z
				RotationDirection::CCW, RotationDirection::CW, // Back
			};

			int mouseButton = -1;
			if (gameContext.pInput->IsMouseButtonPressed(VK_RBUTTON))
			{
				mouseButton = 0;
			}
			else if (gameContext.pInput->IsMouseButtonPressed(VK_LBUTTON))
			{
				mouseButton = 1;
			}

			if (mouseButton != -1)
			{
				RotationDirection rotateDirection = rotationDirecions[(int)layer * 2 + mouseButton];
				AddLayerRotationCommand(layer, rotateDirection);
			}
		}
	}

	if (gameContext.pInput->IsKeyboardKeyPressed('S')) Scramble();
	else if (gameContext.pInput->IsKeyboardKeyPressed('R')) Reset();

	if (m_IsRotatingLayer)
	{
		if (m_LayerRotationsToExecute.size() >= 1)
		{
			RotateActiveLayer();
		}
		else
		{
			m_IsRotatingLayer = false;
		}
	}
}

void RubiksCube::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void RubiksCube::Scramble()
{
	size_t numRotations = 20;
	for (size_t i = 0; i < numRotations; i++)
	{
		Layer layer;
		if (i > 0)
		{
			do
			{
				layer = Layer(rand() % 9);
			} while (layer == m_LayerRotationsToExecute.back().m_RotatingLayer);
		}
		else
		{
			layer = Layer(rand() % 9);
		}
		RotationDirection rotDir = RotationDirection(rand() % 2);
		AddLayerRotationCommand(layer, rotDir);

		if (rand() % 10 == 1)
		{
			// Do a double rotation 10% of the time
			AddLayerRotationCommand(layer, rotDir);
		}
	}
}

void RubiksCube::Reset()
{
	m_HighlightedLayer = Layer::NONE;
	m_FramesOfLayerRotationRemaining = 0;
	m_IsRotatingLayer = false;
	
	// Clear queue
	std::queue<LayerRotationCommand> emptyQueue;
	std::swap(m_LayerRotationsToExecute, emptyQueue);

	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		GameObject* cubeCollider = m_CubeColliders[i];
		SafeDelete(m_CubeColliders[i]);
		RemoveChild(cubeCollider);

		GameObject* cube = m_Cubes[i];
		SafeDelete(m_Cubes[i]);
		RemoveChild(cube);

	}

	CreateCubes();
}

void RubiksCube::RotateCube(int axis, RotationDirection direction)
{
	// TODO: Make layers rotate in sync with each other
	AddLayerRotationCommand(Layer(axis * 3), direction);
	AddLayerRotationCommand(Layer(axis * 3 + 1), direction);
	AddLayerRotationCommand(Layer(axis * 3 + 2), direction);
}

void RubiksCube::AddLayerRotationCommand(LayerRotationCommand command)
{
	AddLayerRotationCommand(command.m_RotatingLayer, command.m_RotatingLayerDirection);
}

void RubiksCube::AddLayerRotationCommand(Layer layer, RotationDirection rotationDirection)
{
	m_LayerRotationsToExecute.push({ layer, rotationDirection });
	if (m_LayerRotationsToExecute.size() == 1)
	{
		m_IsRotatingLayer = true;
		m_FramesOfLayerRotationRemaining = m_FramesOfLayerRotation;
	}
}

void RubiksCube::RotateActiveLayer()
{
	if (m_LayerRotationsToExecute.empty())
	{
		m_IsRotatingLayer = false;
	}

	if (!m_IsRotatingLayer)
	{
		return;
	}

	LayerRotationCommand activeLayerRotationCommand = m_LayerRotationsToExecute.front();

	Layer layer = activeLayerRotationCommand.m_RotatingLayer;
	RotationDirection rotationDirection = activeLayerRotationCommand.m_RotatingLayerDirection;

	RotateLayer(layer, rotationDirection);
}

std::vector<Cubelet*> RubiksCube::GetLayerCubes(Layer layer)
{
	std::vector<Cubelet*> layerCubes;

	Layer currentLayer = layer;
	for (size_t i = 0; i < 9; ++i)
	{
		layerCubes.push_back(m_Cubes[CUBE_INDICIES[int(currentLayer)][i]]);
	}

	return layerCubes;
}

void RubiksCube::RotateLayer(Layer layer, RotationDirection rotationDirection)
{
	// Which cubes are on this layer?
	Cubelet* layerCubes[9];
	for (size_t i = 0; i < 9; ++i)
	{
		layerCubes[i] = m_Cubes[CUBE_INDICIES[int(layer)][i]];
	}

	static const XMFLOAT3 axes[] =
	{
		{ 0, 1, 0 }, // Top, Middle, Bottom
		{ 1, 0, 0 }, // Left, Middle, Right
		{ 0, 0, 1 }, // Front, Middle, Back
	};

	const XMFLOAT3 axis = axes[int(layer) / 3];
	const float angle = (rotationDirection == RotationDirection::CW ? -XM_PIDIV2 : XM_PIDIV2) * (1.0f / m_FramesOfLayerRotation);

	for (size_t i = 0; i < 9; ++i)
	{
		XMFLOAT3 offset = (layerCubes[i]->GetTransform()->GetPosition()) - XMFLOAT3{ 0.5f, 0.2f, 0.5f };
		layerCubes[i]->GetTransform()->Rotate(offset, { axis.x * angle, axis.y * angle, axis.z * angle });
	}

	--m_FramesOfLayerRotationRemaining;
	if (m_FramesOfLayerRotationRemaining == 0)
	{
		RecalculateCubePointers(); // The layers we know are now incorrect, they must be updated to the new state of the cube

		m_LayerRotationsToExecute.pop();
		if (m_LayerRotationsToExecute.size() >= 1)
		{
			m_FramesOfLayerRotationRemaining = m_FramesOfLayerRotation;
		}
		else
		{
			m_IsRotatingLayer = false;
		}
	}
}

void RubiksCube::RecalculateCubePointers()
{
	static const XMUINT2 mapping[9][9] = {
		{ // Top
			{ 6, 24 },{ 24, 26 },{ 26, 8 },{ 8, 6 },{ 0, 0 },
			{ 7, 15 },{ 15, 25 },{ 25, 17 },{ 17, 7 }
		},
		{ // Middle Y
			{ 3, 21 },{ 21, 23 },{ 23, 5 },{ 5, 3 },{ 0, 0 },
			{ 4, 12 },{ 12, 22 },{ 22, 14 },{ 14, 4 }
		},
		{ // Bottom
			{ 0, 18 },{ 18, 20 },{ 20, 2 },{ 2, 0 },{ 0, 0 },
			{ 1, 9 },{ 9, 19 },{ 19, 11 },{ 11, 1 }
		},
		{ // Left
			{ 18, 0 },{ 24, 18 },{ 6, 24 },{ 0, 6 },{ 0, 0 },
			{ 9, 3 },{ 21, 9 },{ 15, 21 },{ 3, 15 }
		},
		{ // Middle X
			{ 19, 1 },{ 25, 19 },{ 7, 25 },{ 1, 7 },{ 0, 0 },
			{ 10, 4 },{ 22, 10 },{ 16, 22 },{ 4, 16 }
		},
		{ // Right
			{ 2, 8 },{ 8, 26 },{ 26, 20 },{ 20, 2 },{ 0, 0 },
			{ 5, 17 },{ 17, 23 },{ 23, 11 },{ 11, 5 }
		},
		{ // Front
			{ 6, 0 },{ 8, 6 },{ 2, 8 },{ 0, 2 },{ 0, 0 },
			{ 3, 1 },{ 7, 3 },{ 5, 7 },{ 1, 5 }
		},
		{ // Middle Z
			{ 15, 9 },{ 17, 15 },{ 11, 17 },{ 9, 11 },{ 0, 0 },
			{ 12, 10 },{ 16, 12 },{ 14, 16 },{ 10, 14 }
		},
		{ // Back
			{ 24, 18 },{ 26, 24 },{ 20, 26 },{ 18, 20 },{ 0, 0 },
			{ 21, 19 },{ 25, 21 },{ 23, 25 },{ 19, 23 }
		}
	};

	// Store pointers to the cubes that are on the rotated layer (in the wrong positions)
	Cubelet* oldCubes[27];
	for (size_t i = 0; i < 27; ++i)
	{
		oldCubes[i] = m_Cubes[i];
	}

	Layer layer = m_LayerRotationsToExecute.front().m_RotatingLayer;
	RotationDirection rotationDirection = m_LayerRotationsToExecute.front().m_RotatingLayerDirection;

	for (size_t i = 0; i < 9; ++i)
	{
		if (i == 4) continue;

		size_t oldIndex = CUBE_INDICIES[int(layer)][i];
		int newIndex = -1;
		for (int j = 0; j < 9; ++j)
		{
			XMUINT2 map = mapping[(int(layer))][j];
			if (rotationDirection == RotationDirection::CCW)
			{
				if (map.y == oldIndex)
				{
					newIndex = map.x;
					break;
				}
			}
			else
			{
				if (map.x == oldIndex)
				{
					newIndex = map.y;
					break;
				}
			}
		}
		assert(newIndex != -1);

		m_Cubes[oldIndex] = oldCubes[newIndex];
	}
}