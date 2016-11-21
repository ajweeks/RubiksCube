
#include "BaseGame.h"

#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

BaseGame::BaseGame() :
	m_hInstance(0),
	m_WindowHandle(0),
	m_RenderTargetWidth(1280),
	m_RenderTargetHeight(720),
	m_WindowTitle(L"RubiksCubeSimulator2k16"),
	m_WindowClassName(L"RubiksCubeSimulator2k16"),
	m_WindowClass(),
	m_Device(nullptr),
	m_pDeviceContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRenderTargetView(nullptr),
	m_pTexDepthStencilBuffer(nullptr),
	m_pDepthStencilView(nullptr),
	m_NumFrames(0),
	m_MousePos({0, 0}),
	m_VBLWait(true)
{
	if(!XMVerifyCPUSupport())	MessageBoxW(0, L"XMVerifyCPUSupport: failed", L"ERROR", MB_OK|MB_ICONERROR);
}

BaseGame::~BaseGame()
{
	CleanupDirect3D();
}

void BaseGame::CleanupDirect3D()
{
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pTexDepthStencilBuffer) m_pTexDepthStencilBuffer->Release();

	if (m_pDeviceContext)
	{
		m_pDeviceContext->ClearState();
		m_pDeviceContext->Release();
	}
	if (m_Device) m_Device->Release();

}

HRESULT BaseGame::RunGame(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	HRESULT hr;

	hr = CreateWindowClass();
	if(FAILED(hr)) return hr; 
	wcout << L"Window Class Created" << endl;

	hr = OpenWindow();
	if(FAILED(hr)) return hr; 
	wcout << L"Window Opened" << endl;

	hr = InitializeDirect3D();
	if(FAILED(hr)) return hr; 
	wcout << L"Direct3D Initialized" << endl;

	hr = GameInitialize();
	if(FAILED(hr)) return hr; 
	wcout << L"BaseGame Initialized" << endl;

	MSG msg = {};
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GameUpdate();
	}
	wcout << L"Finished" << endl;

	return (int)msg.wParam;
}

HRESULT BaseGame::CreateWindowClass()
{
	ZeroMemory( &m_WindowClass, sizeof( WNDCLASS ) );
	m_WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WindowClass.hIcon = nullptr;
	m_WindowClass.hbrBackground = nullptr;
	m_WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	m_WindowClass.lpfnWndProc = WindowsProcedureStatic;
	m_WindowClass.hInstance = GetModuleHandle(0);
	m_WindowClass.lpszClassName = m_WindowClassName.c_str();

	if(!RegisterClass( &m_WindowClass))
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}
	return S_OK;
}

HRESULT BaseGame::OpenWindow()
{
	//Calculate position (centered)
	RECT R = {0,0,m_RenderTargetWidth,m_RenderTargetHeight};
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	LONG windowWidth = R.right - R.left;
	LONG windowHeight = R.bottom - R.top;

	//Calculate the position of the window to be centered on the desktop
	int posX = GetSystemMetrics(SM_CXSCREEN)/2-windowWidth/2;		//calculate the left position of the window
	int posY = GetSystemMetrics(SM_CYSCREEN)/2-windowHeight/2;	//calculate the top position of the window

	
	m_WindowHandle = CreateWindowW( m_WindowClassName.c_str(),
		m_WindowTitle.c_str(), 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 
		posX, 
		posY, 
		windowWidth, 
		windowHeight, 
		NULL, 
		0, 
		m_hInstance, 
		this);//'this' is retrieved in WindowsProcedureStatic by lpCreateParams 

	if(m_WindowHandle == NULL)
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}

	//Show the window
	ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
	return S_OK;
}

HRESULT BaseGame::InitializeDirect3D()
{
	HRESULT hr;

	//Create DXGIFactory
	IDXGIFactory* pDXGIFactory;
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory));
	if(AlertOnFail(hr, L"InitializeDirect3D: ::CreateDXGIFactory(...)"))return hr;

	//Allows DXGI to monitor an application's message queue for the alt-enter key sequence 
	//(which causes the application to switch from windowed to full screen or vice versa).
	//hr = pDXGIFactory->MakeWindowAssociation(m_WindowHandle, 0);
	//if(AlertOnFail(hr, L"InitializeDirect3D: ::MakeWindowAssociation(...)"))return hr;

	//Get all Graphics Adapters
	IDXGIAdapter* pDXGIAdapter = nullptr;
	UINT i = 0;
	while(pDXGIFactory->EnumAdapters(i, &pDXGIAdapter) != DXGI_ERROR_NOT_FOUND) 
	{ 
		DXGI_ADAPTER_DESC adapterDescriptor;
		pDXGIAdapter->GetDesc(&adapterDescriptor);
		wcout << L"Adapter "<< i << L": " << adapterDescriptor.Description <<  endl;
		pDXGIAdapter->Release();
		++i; 
	} 
	wcout << i << L" Adapter(s) found" << endl;

	// Could use the first adapter
	hr = pDXGIFactory->EnumAdapters(0, &pDXGIAdapter);

	//4.2.1 Create the Device and Context
	UINT createDeviceFlags = 0;

	#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0  ;
	hr = D3D11CreateDevice(		pDXGIAdapter,//when pDXGIAdapter is used, nex param must be D3D_DRIVER_TYPE_UNKNOWN
								D3D_DRIVER_TYPE_UNKNOWN,
								NULL,
								createDeviceFlags,
								0,0,
								D3D11_SDK_VERSION,
								&m_Device,
								&featureLevel,
								&m_pDeviceContext
	);
	pDXGIAdapter->Release();
	if(AlertOnFail(hr, L"InitializeDirect3D: D3D11CreateDevice(...)"))return hr;
	if(featureLevel!=D3D_FEATURE_LEVEL_11_0)
		if(AlertOnFail(S_FALSE, L"InitializeDirect3D: Feature level 11.0 unsupported"))return hr;

	//4.2.3 Describe the swapchain
	//IDXGIDevice* pDXGIDevice = nullptr;
	//hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
	//if(AlertOnFail(hr, L"InitializeDirect3D: m_pDevice->QueryInterface pDXGIDevice"))return hr;

	//Create Swapchain descriptor
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc.Width = m_RenderTargetWidth;
	swapChainDesc.BufferDesc.Height = m_RenderTargetHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_WindowHandle;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = pDXGIFactory->CreateSwapChain(m_Device, &swapChainDesc, &m_pSwapChain);

	//release temporary resources
	pDXGIFactory->Release();

	//4.2.5 Create the Render Target View
	ID3D11Texture2D *pRenderTargetBuffer(nullptr);

	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pRenderTargetBuffer));
	if(AlertOnFail(hr, L"InitializeDirect3D: m_pSwapChain->GetBuffer"))return hr;

	hr = m_Device->CreateRenderTargetView(pRenderTargetBuffer, 0, &m_pRenderTargetView);
	if(AlertOnFail(hr, L"InitializeDirect3D: m_pDevice->CreateRenderTargetView"))return hr;

	if(pRenderTargetBuffer)pRenderTargetBuffer->Release();

	//4.2.6 Create the Depth/Stencil Buffer and View
	D3D11_TEXTURE2D_DESC depthStencilDesc={0};
	depthStencilDesc.Width     = m_RenderTargetWidth;
	depthStencilDesc.Height    = m_RenderTargetHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	hr = m_Device->CreateTexture2D(&depthStencilDesc, 0, &m_pTexDepthStencilBuffer);
	if(AlertOnFail(hr, L"InitializeDirect3D: m_pDevice->CreateTexture2D  m_pTexDepthStencilBuffer"))return hr;
	
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc={};
    depthStencilViewDesc.Format = depthStencilDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = m_Device->CreateDepthStencilView(m_pTexDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if(AlertOnFail(hr, L"InitializeDirect3D: m_pDevice->CreateDepthStencilView "))return hr;
		
	//4.2.7 Bind the Views to the Output Merger Stage
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
	
	//4.2.8 Set the Viewport
	m_Viewport.Width	= (FLOAT)m_RenderTargetWidth;
	m_Viewport.Height	= (FLOAT)m_RenderTargetHeight;
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1,&m_Viewport);

	return S_OK;
}

#pragma region
bool BaseGame::AlertOnFail(HRESULT hr, wstring originInfo)
{
	if(SUCCEEDED(hr)) return false;

	//Extract error message from HRESULT
	if(FACILITY_WINDOWS == HRESULT_FACILITY(hr)) 
		hr = HRESULT_CODE(hr); 

	wstringstream ss;
	if(originInfo.size() != 0)
	{
		ss<<">ORIGIN:\n";
		ss<<originInfo;
		ss<<"\n\n";
	}
	ss<<"ERROR:\n";

	TCHAR* errorMsg; 
	if(FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPTSTR)&errorMsg, 0, NULL) != 0) 
	{ 
		ss<<errorMsg;
	} 
	else
	{
		ss<<"Could not find a description for error ";
		ss<<hr;
		ss<<".";		 
	}

	MessageBox(0, ss.str().c_str(), L"ERROR", MB_OK|MB_ICONERROR);

	return true;
}
#pragma endregion AlertOnFail

#pragma region
LRESULT CALLBACK BaseGame::WindowsProcedureStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_CREATE)
	{
		CREATESTRUCT *pCS = (CREATESTRUCT*)lParam;
		SetWindowLongW(hWnd, GWLP_USERDATA, (LONG)pCS->lpCreateParams);
	}
	else
	{
		BaseGame* pThisGame = (BaseGame*)GetWindowLongW(hWnd, GWLP_USERDATA);
		if(pThisGame) return pThisGame->WindowsProcedure(hWnd, message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

XMINT2 BaseGame::GetWindowSize() const
{
	RECT rect;
	GetWindowRect(m_WindowHandle, &rect);

	return XMINT2(rect.right - rect.left, rect.bottom - rect.top);
}

LRESULT BaseGame::WindowsProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		OnKeyDown(wParam);
	} break;
	case WM_KEYUP:
	{
		OnKeyUp(wParam);
	} break;
	case WM_MOUSEMOVE:
	{
		m_MousePos.x = GET_X_LPARAM(lParam);
		m_MousePos.y = GET_Y_LPARAM(lParam);
	} break;
	case WM_MOUSEWHEEL:
	{
		OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
	}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

#pragma endregion Windows Procedures