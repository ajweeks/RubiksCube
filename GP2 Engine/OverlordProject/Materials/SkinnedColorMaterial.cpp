
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkinnedColorMaterial.h"
#include "Base\GeneralStructs.h"
#include "Components\ModelComponent.h"
#include "Graphics\ModelAnimator.h"

ID3DX11EffectMatrixVariable* SkinnedColorMaterial::m_pBoneTransformsVariable = nullptr;

SkinnedColorMaterial::SkinnedColorMaterial(bool enableTransparency) :
	Material(L"./Resources/Effects/PosNormCol3D_Skinned.fx", enableTransparency ? L"TransparencyTech" : L"")
{
}

SkinnedColorMaterial::~SkinnedColorMaterial()
{
}

void SkinnedColorMaterial::LoadEffectVariables()
{
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

void SkinnedColorMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto boneTransforms = pModelComponent->GetAnimator()->GetBoneTransforms();
	if (m_pBoneTransformsVariable && !boneTransforms.empty())
	{
		XMFLOAT4X4* data = boneTransforms.data();
		const float* dataF = (float*)data;

		m_pBoneTransformsVariable->SetMatrixArray(dataF, 0, boneTransforms.size());
	}
}
