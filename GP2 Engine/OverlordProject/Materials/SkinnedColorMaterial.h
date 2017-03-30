#pragma once
#include "Graphics\Material.h"

class SkinnedColorMaterial : public Material
{
public:
	SkinnedColorMaterial(bool enableTransparency = false);
	~SkinnedColorMaterial();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	static ID3DX11EffectMatrixVariable* m_pBoneTransformsVariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkinnedColorMaterial(const SkinnedColorMaterial &obj);
	SkinnedColorMaterial& operator=(const SkinnedColorMaterial& obj);
};
