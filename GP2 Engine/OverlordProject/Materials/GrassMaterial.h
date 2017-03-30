#pragma once
#include "Graphics\Material.h"

class TextureData;

class GrassMaterial : public Material
{
public:
	GrassMaterial();
	~GrassMaterial();

	void SetDiffuseTexture(const wstring& assetFile);

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GrassMaterial(const GrassMaterial &obj);
	GrassMaterial& operator=(const GrassMaterial& obj);
};

