//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GrassMaterial.h"
#include "Base/GeneralStructs.h"
#include "Diagnostics/Logger.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"

ID3DX11EffectShaderResourceVariable* GrassMaterial::m_pDiffuseSRVvariable = nullptr;

GrassMaterial::GrassMaterial() : Material(L"./Resources/Effects/Grass.fx"),
m_pDiffuseTexture(nullptr)
{
}


GrassMaterial::~GrassMaterial()
{

}

void GrassMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void GrassMaterial::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("m_TextureDiffuse")->AsShaderResource();

		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'m_TextureDiffuse\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}
}

void GrassMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}
}
