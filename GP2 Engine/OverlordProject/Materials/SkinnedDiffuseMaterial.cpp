//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkinnedDiffuseMaterial.h"
#include "Base/GeneralStructs.h"
#include "Diagnostics/Logger.h"
#include "Content/ContentManager.h"
#include "Graphics/TextureData.h"
#include "Components/ModelComponent.h"
#include "Graphics/ModelAnimator.h"

ID3DX11EffectShaderResourceVariable* SkinnedDiffuseMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectMatrixVariable* SkinnedDiffuseMaterial::m_pBoneTransformsVariable = nullptr;

SkinnedDiffuseMaterial::SkinnedDiffuseMaterial() : Material(L"./Resources/Effects/PosNormTex3D_Skinned.fx"),
	m_pDiffuseTexture(nullptr)
{
}

SkinnedDiffuseMaterial::~SkinnedDiffuseMaterial()
{
}

void SkinnedDiffuseMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkinnedDiffuseMaterial::LoadEffectVariables()
{
	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'gDiffuseMap\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}

	if (!m_pBoneTransformsVariable)
	{
		m_pBoneTransformsVariable = m_pEffect->GetVariableByName("gBones")->AsMatrix();
		if (!m_pBoneTransformsVariable->IsValid())
		{
			Logger::LogWarning(L"SkinnedDiffuseMaterial::LoadEffectVariables() > \'gBones\' variable not found!");
			m_pBoneTransformsVariable = nullptr;
		}
	}
}

void SkinnedDiffuseMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	if (m_pDiffuseTexture && m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	}

	auto boneTransforms = pModelComponent->GetAnimator()->GetBoneTransforms();
	if (m_pBoneTransformsVariable && !boneTransforms.empty())
	{
		XMFLOAT4X4* data = boneTransforms.data();
		const float* dataF = (float*)data;

		m_pBoneTransformsVariable->SetMatrixArray(dataF, 0, boneTransforms.size());
	}
}
