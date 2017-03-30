#pragma once
#include "Graphics\Material.h"

class TextureData;

class SkinnedDiffuseMaterial : public Material
{
public:
	SkinnedDiffuseMaterial();
	~SkinnedDiffuseMaterial();

	void SetDiffuseTexture(const wstring& assetFile);

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:

	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	static ID3DX11EffectMatrixVariable* m_pBoneTransformsVariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkinnedDiffuseMaterial(const SkinnedDiffuseMaterial &obj);
	SkinnedDiffuseMaterial& operator=(const SkinnedDiffuseMaterial& obj);
};

