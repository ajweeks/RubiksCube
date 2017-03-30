
#include "stdafx.h"

#include "Materials\CubeMapMaterial.h"
#include "Components\ModelComponent.h"
#include "Base\GeneralStructs.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"

ID3DX11EffectShaderResourceVariable* CubeMapMaterial::m_pCubemapVariable = nullptr;

CubeMapMaterial::CubeMapMaterial(const wstring& textureFilepath) :
	Material(L"Resources/Effects/SkyBox.fx", L"Render")
{
	SetTexture(textureFilepath);
}

CubeMapMaterial::~CubeMapMaterial()
{
}

void CubeMapMaterial::SetTexture(const std::wstring& assetfile)
{
	m_pTextureData = ContentManager::Load<TextureData>(assetfile);
}

void CubeMapMaterial::LoadEffectVariables()
{
	m_pCubemapVariable = m_pEffect->GetVariableByName("m_CubeMap")->AsShaderResource();
}

void CubeMapMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	m_pCubemapVariable->SetResource(m_pTextureData->GetShaderResourceView());
}
