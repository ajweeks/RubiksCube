#pragma once

#include "Graphics\Material.h"

class TextureData;

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial();
	virtual ~DiffuseMaterial();

	void SetDiffuseTexture(const std::wstring& assetfile);

private:
	virtual void LoadEffectVariables() override;
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

	TextureData* m_pTextureData;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVariable;

	DiffuseMaterial(const DiffuseMaterial&) = delete;
	DiffuseMaterial& operator=(const DiffuseMaterial&) = delete;
};

