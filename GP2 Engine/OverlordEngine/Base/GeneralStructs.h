#pragma once
#include "GameTime.h"
#include "../Components/CameraComponent.h"
#include "InputManager.h"
#include "MaterialManager.h"

class CameraComponent;

struct GameSettings
{
public:
	GameSettings():
		Window(WindowSettings()),
		DirectX(DirectXSettings())
	{}

#pragma region
	struct WindowSettings
	{
		WindowSettings():
			Width(1280),
			Height(720),
			AspectRatio(Width/(float)Height),
			Title(L"Overlord Engine (DX11)"),
			WindowHandle(nullptr)
		{
		}

		int Width;
		int Height;
		float AspectRatio;
		wstring Title;
		HWND WindowHandle;
	}Window;
#pragma endregion WINDOW_SETTINGS

#pragma region
	struct DirectXSettings
	{
		DirectXSettings():
			pAdapter(nullptr),
			pOutput(nullptr)
		{}

		IDXGIAdapter* pAdapter;
		IDXGIOutput* pOutput;
	}DirectX;
#pragma endregion DIRECTX_SETTINGS
};

struct GameContext
{
public:
	GameTime* pGameTime;
	CameraComponent* pCamera;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	InputManager* pInput;
	MaterialManager* pMaterialManager;
};
