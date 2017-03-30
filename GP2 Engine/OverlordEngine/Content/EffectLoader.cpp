//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "EffectLoader.h"

EffectLoader::EffectLoader(void){}
EffectLoader::~EffectLoader(void){}

ID3DX11Effect* EffectLoader::LoadContent(const wstring& assetFile)
{
	HRESULT hr;
	ID3D10Blob* pErrorBlob = nullptr;
	ID3DX11Effect* pEffect;
	
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	hr = D3DX11CompileEffectFromFile(assetFile.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		m_pDevice,
		&pEffect,
		&pErrorBlob);

	if(FAILED(hr))
	{
		if(pErrorBlob!=nullptr)
		{
			char *errors = (char*)pErrorBlob->GetBufferPointer();
 
			wstringstream ss;
			for(unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
				ss<<errors[i];
 
			OutputDebugStringW(ss.str().c_str());
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			Logger::LogError(ss.str());
		}
		else
		{
			wstringstream ss;
			ss<<"EffectLoader: Failed to CreateEffectFromFile!\nPath: ";
			ss<<assetFile;
			Logger::LogHResult(hr, ss.str());
			return nullptr;
		}
	}

	return pEffect;
}

void EffectLoader::Destroy(ID3DX11Effect* objToDestroy)
{
	SafeRelease(objToDestroy);
}
