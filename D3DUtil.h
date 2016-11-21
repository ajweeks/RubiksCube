//Direct3D Header Files:
#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//DirectXMath Header Files:
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
using namespace DirectX;

//util lib for loading effects
#include "AdditionalLibraries/DX_Effects11/include/d3dx11effect.h"
#include "AdditionalLibraries/DX_Error/include/DXErr.h"
#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment(lib, "AdditionalLibraries/DX_Effects11/lib/Debug/DxEffects11_d.lib")
	#pragma comment(lib, "AdditionalLibraries/DX_Error/lib/Debug/DXErr_d.lib")
#else 
	#pragma comment(lib, "AdditionalLibraries/DX_Effects11/lib/Release/DxEffects11.lib")
	#pragma comment(lib, "AdditionalLibraries/DX_Error/lib/Release/DXErr.lib")
#endif


