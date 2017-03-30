#pragma once

#include "../../OverlordEngine/Graphics/Material.h"

class TextureData;

class CubeMapMaterial : public Material
{
public:
	CubeMapMaterial(const wstring& textureFilepath);
	virtual ~CubeMapMaterial();

	void SetTexture(const std::wstring& textureFilepath);

private:
	virtual void LoadEffectVariables() override;
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

	TextureData* m_pTextureData;
	static ID3DX11EffectShaderResourceVariable* m_pCubemapVariable;

	CubeMapMaterial(const CubeMapMaterial&) = delete;
	CubeMapMaterial& operator=(const CubeMapMaterial&) = delete;
};

