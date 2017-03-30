
#include "stdafx.h"

#include "Materials\DiffuseMaterial.h"
#include "Components\ModelComponent.h"
#include "Base\GeneralStructs.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"

ID3DX11EffectShaderResourceVariable* DiffuseMaterial::m_pDiffuseSRVariable = nullptr;

DiffuseMaterial::DiffuseMaterial() :
	Material(L"Resources/Effects/PosNormTex3D.fx", L"Default")
{
}

DiffuseMaterial::~DiffuseMaterial()
{
}

void DiffuseMaterial::SetDiffuseTexture(const std::wstring& assetfile)
{
	m_pTextureData = ContentManager::Load<TextureData>(assetfile);
}

void DiffuseMaterial::LoadEffectVariables()
{
	m_pDiffuseSRVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

void DiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);
	m_pDiffuseSRVariable->SetResource(m_pTextureData->GetShaderResourceView());
}
