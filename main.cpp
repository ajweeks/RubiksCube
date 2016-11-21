
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <crtdbg.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

//ReportLiveObjects:
//To use any of these GUID values, you must include Initguid.h before you include DXGIDebug.h in your code.
#include <Initguid.h>
#include <dxgidebug.h>

#include "RubiksCube.h"

int wmain(int argc, wchar_t* argv[])
{
	return wWinMain(GetModuleHandle(0), 0, 0, SW_SHOW);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   wchar_t* cmdLine, int showCmd)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL,0);

	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

		typedef HRESULT(__stdcall *fPtr)(const IID&, void**); 
		HMODULE hDll = LoadLibrary(L"dxgidebug.dll"); 
		fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface"); 

		IDXGIDebug* pDXGIDebug;
		DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDXGIDebug);
		//_CrtSetBreakAlloc(301);
	#endif

	BaseGame *gamePtr = nullptr;

	//Third Lab: 3D
	gamePtr = new RubiksCube(); 

	//Run and delete
	gamePtr->RunGame(hInstance);
	delete gamePtr;

#if defined(DEBUG) | defined(_DEBUG)
	if(pDXGIDebug) pDXGIDebug->ReportLiveObjects(DXGI_DEBUG_ALL,DXGI_DEBUG_RLO_ALL); 
    pDXGIDebug->Release();
#endif

}
