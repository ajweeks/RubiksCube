#pragma once
#include "GeneralStructs.h"

class RenderTarget;
class GameTime;

class OverlordGame
{
public:
	OverlordGame(void);
	~OverlordGame(void);

	static LRESULT CALLBACK WindowsProcedureStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT Run(HINSTANCE hInstance);

	//Methods
	static const GameSettings& GetGameSettings() { return m_GameSettings; }
	void SetRenderTarget(RenderTarget* renderTarget);
	// Update PP
	RenderTarget* GetRenderTarget() const;

	void SetClearColor(XMFLOAT4 clearColor);

protected:

	virtual void OnGamePreparing(GameSettings& gameSettings){ UNREFERENCED_PARAMETER(gameSettings); }
	virtual LRESULT WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void Initialize() = 0;
	//virtual void Update(GameContext context) = 0;
	//virtual void Draw(GameContext context)Set = 0;
	bool m_IsActive;

private:

	//FUNCTIONS
	//Initializations
	HRESULT InitializeAdapterAndOutput();
	HRESULT InitializeWindow();
	HRESULT InitializeDirectX();
	HRESULT InitializePhysX() const;
	HRESULT InitializeGame();
	void GameLoop() const;

	//Windows Proc
	void StateChanged(int state, bool active);
	LRESULT WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//MEMBERS
	static GameSettings m_GameSettings;

	XMFLOAT4 m_ClearColor;

	HINSTANCE m_hInstance;
	HWND m_WindowHandle;	
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapchain;
	IDXGIFactory* m_pDxgiFactory;
	RenderTarget* m_pDefaultRenderTarget, *m_pCurrentRenderTarget; 
	D3D11_VIEWPORT m_Viewport;
};

