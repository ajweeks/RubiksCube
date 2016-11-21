#pragma once
// Windows Header Files:
#include <windowsx.h>
#include <string>

#include "D3DUtil.h"


class BaseGame
{
public:
	BaseGame();
	virtual ~BaseGame();
	HRESULT RunGame(HINSTANCE hInstance);

	static LRESULT CALLBACK WindowsProcedureStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	XMINT2 GetWindowSize() const;

protected:
	virtual void OnKeyDown(int vkCode) {};
	virtual void OnKeyUp(int vkCode) {};
	virtual void OnMouseWheel(int scrollAmount) {};

	// Direct3D variables
	ID3D11Device *m_Device;
	ID3D11DeviceContext *m_pDeviceContext;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11Texture2D *m_pTexDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;
	IDXGISwapChain *m_pSwapChain;
	D3D11_VIEWPORT m_Viewport;

	int m_NumFrames;
	bool m_VBLWait;
	XMINT2 m_MousePos;

private:
	// Functions
	HRESULT CreateWindowClass();
	HRESULT OpenWindow();
	HRESULT InitializeDirect3D();

	// Game specific pure virtual functions
	virtual HRESULT GameInitialize() = 0;
	virtual void GameUpdate() = 0;

	void CleanupDirect3D();

	// Handle Windows Messages
	LRESULT WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Report failures
	bool AlertOnFail(HRESULT hr, std::wstring originInfo = L"");

	// Window variables
	HINSTANCE m_hInstance;
	HWND m_WindowHandle;
	int m_RenderTargetWidth, m_RenderTargetHeight;
	std::wstring m_WindowClassName;
	WNDCLASS m_WindowClass;
	std::wstring m_WindowTitle;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BaseGame(const BaseGame& t);
	BaseGame& operator=(const BaseGame& t);
};

