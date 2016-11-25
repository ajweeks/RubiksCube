
#include "RubiksCube.h"
#include "CubePosCol3D.h"
#include "QuadPosCol3D.h"
#include "SpherePosCol3D.h"
#include "Quad3D.h"

#include <sstream>
#include <iostream>
#include <vector>
using namespace DirectX;

XMFLOAT3 RubiksCube::s_CubePositions[27];

const float RubiksCube::CUBE_WIDTH = 0.3f;
const float RubiksCube::SPACING = 0.05f;

RubiksCube::RubiksCube() :
	BaseGame()
{
	SetRotsAndDirs();

	for (size_t i = 0; i < NUM_CUBES; ++i)
		m_Cubes[i] = nullptr;
	
	m_BackdropCube = nullptr;
	m_ControlsPlane = nullptr;
}

RubiksCube::~RubiksCube()
{
	for (size_t i = 0; i < NUM_CUBES; ++i)
		delete m_Cubes[i];
	
	delete m_BackdropCube;

	for (size_t i = 0; i < m_Spheres.size(); ++i)
	{
		delete m_Spheres[i];
	}
	m_Spheres.clear();

	delete m_ControlsPlane;
	
	if (m_RasterizerState) m_RasterizerState->Release();
	Shape::ReleaseAllEffects();
}

void RubiksCube::SetRotsAndDirs()
{
	for (size_t i = 0; i < NUM_CUBES; ++i)
	{
		const float cubeX = (i % 3) * (CUBE_WIDTH + SPACING);
		const float cubeY = ((i / 3) % 3) * (CUBE_WIDTH + SPACING);
		const float cubeZ = (i / 9) * (CUBE_WIDTH + SPACING);

		s_CubePositions[i] = XMFLOAT3{ cubeX, cubeY, cubeZ };
	}
}

HRESULT RubiksCube::GameInitialize()
{
	HRESULT hr;
	
	Shape::BuildAllEffects(m_Device);
	
	const XMFLOAT4 RED =	{ 0.9f, 0.0f, 0.0f, 1.0f };
	const XMFLOAT4 GREEN =	{ 0.0f, 1.0f, 0.0f, 1.0f };
	const XMFLOAT4 BLUE =	{ 0.0f, 0.0f, 1.0f, 1.0f };
	const XMFLOAT4 YELLOW = { 1.0f, 1.0f, 0.0f, 1.0f };
	const XMFLOAT4 ORANGE = { 0.9f, 0.4f, 0.0f, 1.0f };
	const XMFLOAT4 PINK =	{ 1.0f, 0.0f, 1.0f, 1.0f };
	const XMFLOAT4 CYAN =	{ 0.0f, 1.0f, 1.0f, 1.0f };
	const XMFLOAT4 WHITE =	{ 1.0f, 1.0f, 1.0f, 1.0f };
	const XMFLOAT4 BLACK = { 0.0f, 0.0f, 0.0f, 1.0f };
	const XMFLOAT4 OFF_BLACK =	{ 0.1f, 0.1f, 0.15f, 1.0f };
	
	m_Spheres.reserve(NUM_CUBES);

	for (size_t i = 0; i < NUM_CUBES; ++i)
	{
		const size_t cubeX = (i % 3);
		const size_t cubeY = ((i / 3) % 3);
		const size_t cubeZ = (i / 9);
		m_Cubes[i] = new CubePosCol3D(m_Device);

		enum class Faces
		{
			FRONT, RIGHT, BACK, LEFT, TOP, BOTTOM
		//  RED, GREEN, ORANGE, BLUE, WHITE, YELLOW
		};

		XMFLOAT4 faceColors[6];
		for (size_t i = 0; i < 6; ++i)
			faceColors[i] = OFF_BLACK;

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

		hr = m_Cubes[i]->Create({ 0, 0, 0 }, CUBE_WIDTH, CUBE_WIDTH, CUBE_WIDTH, faceColors[0], faceColors[1], faceColors[2], faceColors[3], faceColors[4], faceColors[5]);
		if (FAILED(hr))
		{
			MessageBox(NULL, _T("Create CubePosCol3D Failed"), _T("CubePosCol3D Fail"), MB_ICONERROR);
		}
		m_Cubes[i]->SetPos(s_CubePositions[i]);


		SpherePosCol3D* sphere = new SpherePosCol3D(m_Device);
		hr = sphere->Create({ 0, 0, 0 }, 0.01f, BLUE, RED, WHITE);
		if (FAILED(hr))
		{
			MessageBox(NULL, _T("Create SpherePosCol3D Failed"), _T("SpherePosCol3D Fail"), MB_ICONERROR);
		}
		sphere->SetPos(s_CubePositions[i]);
		m_Spheres.push_back(sphere);
	}
	
	{
		static const float width = 15.0f;
		static const float height = 15.0f;
		static const float depth = 15.0f;
	
		m_BackdropCube = new CubePosCol3D(m_Device);
		hr = m_BackdropCube->Create({ 0, 0, 0 }, width, height, depth,
		{ 0.45f, 0.2f, 0.49f, 1.0f },
		{ 0.65f, 0.5f, 0.15f, 1.0f },
		{ 0.15f, 0.3f, 0.29f, 1.0f },
		{ 0.2f, 0.14f, 0.49f, 1.0f },
		{ 0.1f, 0.11f, 0.09f, 1.0f },
		{ 0.07f, 0.1f, 0.09f, 1.0f });
		m_BackdropCube->SetPos({ -width / 2.0f, height / 2.0f, -depth * 0.6f });
	}

	m_ControlsPlane = new Quad3D(m_Device);
	hr = m_ControlsPlane->Create(L"res/controls.dds", { 0, 0, 0 }, 1.0f, 1.0f);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("Create Quad3D Failed"), _T("Quad3D Fail"), MB_ICONERROR);
	}
	m_ControlsPlane->SetScale({ 3.0f, 3.0f, 3.0f });
	m_ControlsPlane->SetPos({ -1.0f, 1.75f, 0.5f });

	SetWireframe(m_Wireframe);

	return S_OK;
}

void RubiksCube::GameUpdate()
{
	const float clearColour[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColour);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pDeviceContext->RSSetState(m_RasterizerState);
	
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

	for (size_t i = 0; i < NUM_CUBES; ++i)
	{
		m_Cubes[i]->Update();
	}
	
	m_ControlsPlane->Update();

	m_Elapsed += 1.0f/60.0f;
	
	const XMINT2 windowSize = GetWindowSize();
	const float windowMovementScale = 5.5f;
	XMFLOAT2 cameraPos = {};
	if (windowSize.x != 0 && windowSize.y != 0) 
	{
		cameraPos = { m_MousePos.x / float(windowSize.x) * windowMovementScale - 2.5f, 
			(1.0f - m_MousePos.y / float(windowSize.y)) * windowMovementScale - 1.5f };
	}
	XMVECTOR pos = XMVectorSet(cameraPos.x, cameraPos.y, m_CameraZ, 0.0f);
	XMVECTOR target = XMVectorSet((CUBE_WIDTH+SPACING) * 1.5f, (CUBE_WIDTH + SPACING) * 1.5f, (CUBE_WIDTH + SPACING) * 1.5f, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX matView = XMMatrixLookAtLH(pos, target, up);
	
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(XM_PIDIV4, float(windowSize.x)/float(windowSize.y), 0.1f, 100);
	
	XMFLOAT4X4 view4x4, proj4x4;
	XMStoreFloat4x4(&view4x4, matView);
	XMStoreFloat4x4(&proj4x4, matProjection);
	

	for (size_t i = 0; i < NUM_CUBES; ++i)
	{	
		//if (i == 7)
		//{
		//	SetWireframe(true);
		//	m_pDeviceContext->RSSetState(m_RasterizerState);
		//	m_Cubes[i]->SetViewMatrix(view4x4);
		//	m_Cubes[i]->SetProjectionMatrix(proj4x4);
		//	m_Cubes[i]->Paint(m_pDeviceContext);
		//	SetWireframe(false);
		//	m_pDeviceContext->RSSetState(m_RasterizerState);
		//}
		//else
		{
			m_Cubes[i]->SetViewMatrix(view4x4);
			m_Cubes[i]->SetProjectionMatrix(proj4x4);
			m_Cubes[i]->Paint(m_pDeviceContext);
		}
	}

	if (m_ShowCubeHelpers)
	{
		for (size_t i = 0; i < m_Spheres.size(); ++i)
		{
			m_Spheres[i]->SetViewMatrix(view4x4);
			m_Spheres[i]->SetProjectionMatrix(proj4x4);
			m_Spheres[i]->Paint(m_pDeviceContext);
		}
	}
	
	{
		D3D11_RASTERIZER_DESC desc = {};
		desc.FillMode = (m_Wireframe ? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID);
		desc.CullMode = D3D11_CULL_FRONT;
		desc.DepthClipEnable = true;
		m_Device->CreateRasterizerState(&desc, &m_RasterizerState);
		m_pDeviceContext->RSSetState(m_RasterizerState);
	
		m_BackdropCube->SetViewMatrix(view4x4);
		m_BackdropCube->SetProjectionMatrix(proj4x4);
		m_BackdropCube->Paint(m_pDeviceContext);
	
		desc.CullMode = D3D11_CULL_BACK;
		m_Device->CreateRasterizerState(&desc, &m_RasterizerState);
		m_pDeviceContext->RSSetState(m_RasterizerState);
	}

	m_ControlsPlane->SetViewMatrix(view4x4);
	m_ControlsPlane->SetProjectionMatrix(proj4x4);
	m_ControlsPlane->Paint(m_pDeviceContext);
	
	++m_NumFrames;
	
	m_pSwapChain->Present(m_VBLWait ? 1 : 0, 0);
}

void RubiksCube::Scramble()
{
}

void RubiksCube::Reset()
{
}

void RubiksCube::RotateCube(int axis, RotationDirection direction)
{
	AddLayerRotationCommand(Layer(axis * 3), direction);
	AddLayerRotationCommand(Layer(axis * 3 + 1), direction);
	AddLayerRotationCommand(Layer(axis * 3 + 2), direction);
}

void RubiksCube::OnKeyDown(int vkCode)
{
	if (vkCode == 'Q') AddLayerRotationCommand(Layer::TOP, RotationDirection::CCW);
	else if (vkCode == 'A') AddLayerRotationCommand(Layer::MIDDLE_Y, RotationDirection::CCW);
	else if (vkCode == 'Z') AddLayerRotationCommand(Layer::BOTTOM, RotationDirection::CCW);

	else if (vkCode == '2') AddLayerRotationCommand(Layer::LEFT, RotationDirection::CCW);
	else if (vkCode == '3') AddLayerRotationCommand(Layer::MIDDLE_X, RotationDirection::CCW);
	else if (vkCode == '4') AddLayerRotationCommand(Layer::RIGHT, RotationDirection::CCW);
		
	else if (vkCode == 'X') AddLayerRotationCommand(Layer::LEFT, RotationDirection::CW);
	else if (vkCode == 'C') AddLayerRotationCommand(Layer::MIDDLE_X, RotationDirection::CW);
	else if (vkCode == 'V') AddLayerRotationCommand(Layer::RIGHT, RotationDirection::CW);
		
	else if (vkCode == 'T') AddLayerRotationCommand(Layer::TOP, RotationDirection::CW);
	else if (vkCode == 'G') AddLayerRotationCommand(Layer::MIDDLE_Y, RotationDirection::CW);
	else if (vkCode == 'B') AddLayerRotationCommand(Layer::BOTTOM, RotationDirection::CW);

	else if (vkCode == 'E') RotateCube(1, RotationDirection::CCW);
	else if (vkCode == 'S') RotateCube(0, RotationDirection::CCW);
	else if (vkCode == 'D') RotateCube(1, RotationDirection::CW);
	else if (vkCode == 'F') RotateCube(0, RotationDirection::CW);
}

void RubiksCube::OnKeyUp(int vkCode)
{
	if (vkCode == 'O') ToggleWireframe();
	else if (vkCode == 'P') m_ShowCubeHelpers = !m_ShowCubeHelpers;
}

void RubiksCube::OnMouseWheel(int scrollAmount)
{
	m_CameraZ += scrollAmount * 0.005f;
}

// TODO: Move to header
static const size_t cubeIndicies[9][9] = {
	{ 6, 7, 8, 15, 16, 17, 24, 25, 26 },	// Top
	{ 3, 4, 5, 12, 13, 14, 21, 22, 23 },	// Middle Y
	{ 0, 1, 2, 9, 10, 11, 18, 19, 20 },		// Bottom
	{ 0, 3, 6, 9, 12, 15, 18, 21, 24 },		// Left
	{ 1, 4, 7, 10, 13, 16, 19, 22, 25 },	// Middle X
	{ 2, 5, 8, 11, 14, 17, 20, 23, 26 },	// Right
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26 },	// Back
	{ 9, 10, 11, 12, 13, 14, 15, 16, 17 },	// Middle Z
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8 }			// Front
};

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
	if (m_LayerRotationsToExecute.size() == 0) 
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

void RubiksCube::RotateLayer(Layer layer, RotationDirection rotationDirection)
{
	// Which cubes are on this layer?
	CubePosCol3D* layerCubes[9];
	for (size_t i = 0; i < 9; ++i)
	{
		layerCubes[i] = m_Cubes[cubeIndicies[int(layer)][i]];
	}

	static const XMFLOAT3 axes[] =
	{
		{ 0, 1, 0 },
		{ 1, 0, 0 },
		{ 0, 0, 1 },
	};

	const XMFLOAT3 axis = axes[int(layer) / 3];
	const float angle = (rotationDirection == RotationDirection::CW ? -XM_PIDIV2 : XM_PIDIV2) * (1.0f / m_FramesOfLayerRotation);

	for (size_t i = 0; i < 9; ++i)
	{
		XMFLOAT3 offset = (layerCubes[i]->GetPosition()) - XMFLOAT3{ 0.5f, 0.2f, 0.5f };
		layerCubes[i]->Rotate(offset, { axis.x * angle, axis.y * angle, axis.z * angle });
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
			{ 6, 24 },{ 24, 26 },{ 26, 8 },{ 8, 6 }, { 0, 0 },
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
			{ 0, 6 },{ 6, 8 },{ 8, 2 },{ 2, 0 },{ 0, 0 },
			{ 1, 3 },{ 3, 7 },{ 7, 5 },{ 5, 1 }
		},
		{ // Middle Z
			{ 9, 15 },{ 15, 17 },{ 17, 11 },{ 11, 9 },{ 0, 0 },
			{ 10, 12 },{ 12, 16 },{ 16, 14 },{ 14, 10 }
		},
		{ // Back
			{ 18, 20 },{ 20, 26 },{ 26, 24 },{ 24, 18 },{ 0, 0 },
			{ 19, 21 },{ 21, 25 },{ 25, 23 },{ 23, 19 }
		}
	};

	// Store pointers to the cubes that are on the rotated layer (in the wrong positions)
	CubePosCol3D* oldCubes[27];
	for (size_t i = 0; i < 27; ++i)
	{
		oldCubes[i] = m_Cubes[i];
	}

	Layer layer = m_LayerRotationsToExecute.front().m_RotatingLayer;
	RotationDirection rotationDirection = m_LayerRotationsToExecute.front().m_RotatingLayerDirection;

	for (size_t i = 0; i < 9; ++i)
	{
		if (i == 4) continue;

		int oldIndex = cubeIndicies[int(layer)][i];
		int newIndex = -1;
		for (int k = 0; k < 9; ++k)
		{
			XMUINT2 map = mapping[(int(layer))][k];
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

void RubiksCube::ToggleWireframe()
{
	SetWireframe(!m_Wireframe);
}

void RubiksCube::SetWireframe(bool wireframe)
{
	m_Wireframe = wireframe;

	D3D11_RASTERIZER_DESC desc = {};
	desc.FillMode = (m_Wireframe ? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID);
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = true;

	if (m_RasterizerState) m_RasterizerState->Release();
	m_Device->CreateRasterizerState(&desc, &m_RasterizerState);
}
