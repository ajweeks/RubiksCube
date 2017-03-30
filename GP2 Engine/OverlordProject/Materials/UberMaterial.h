#pragma once
#include "Graphics\Material.h"

class TextureData;

class UberMaterial : public Material
{
public:
	UberMaterial();
	~UberMaterial();

	//LIGHT
	//*****
	void SetLightDirection(XMFLOAT3 direction);
	
	//DIFFUSE
	//*******
	void EnableDiffuseTexture(bool enable);
	void SetDiffuseTexture(const wstring& assetFile);
	void SetDiffuseColor(XMFLOAT4 color);
	
	//SPECULAR
	//********
	void SetSpecularColor(XMFLOAT4 color);
	void EnableSpecularLevelTexture(bool enable);
	void SetSpecularLevelTexture(const wstring& assetFile);
	void SetShininess(int shininess);
	
	//AMBIENT
	//*******
	void SetAmbientColor(XMFLOAT4 color);
	void SetAmbientIntensity(float intensity);

	//NORMAL MAPPING
	//**************
	void FlipNormalGreenCHannel(bool flip);
	void EnableNormalMapping(bool enable);
	void SetNormalMapTexture(const wstring& assetFile);
	
	//ENVIRONMENT MAPPING
	//*******************
	void EnableEnvironmentMapping(bool enable);
	void SetEnvironmentCube(const wstring& assetFile);
	void SetReflectionStrength(float strength);
	void SetRefractionStrength(float strength);
	void SetRefractionIndex(float index);

	//OPACITY
	//*******
	void SetOpacity(float opacity);
	void EnableOpacityMap(bool enable);
	void SetOpacityTexture(const wstring& assetFile);

	//SPECULAR MODELS
	//***************
	void EnableSpecularBlinn(bool enable);
	void EnableSpecularPhong(bool enable);

	//FRESNEL FALLOFF
	//***************
	void EnableFresnelFaloff(bool enable);
	void SetFresnelColor(XMFLOAT4 color);
	void SetFresnelPower(float power);
	void SetFresnelMultiplier(float multiplier);
	void SetFresnelHardness(float hardness);

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:

	//LIGHT
	//*****
	XMFLOAT3 m_LightDirection;
	static ID3DX11EffectVectorVariable* m_pLightDirectionVariable;

	//DIFFUSE
	//*******
	bool m_bDiffuseTexture;
	static ID3DX11EffectScalarVariable* m_pUseDiffuseTextureVariable;
	TextureData* m_pDiffuseTexture = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;
	XMFLOAT4 m_ColorDiffuse;
	static ID3DX11EffectVectorVariable* m_pDiffuseColorVariable;

	//SPECULAR
	//********
	XMFLOAT4 m_ColorSpecular;
	static ID3DX11EffectVectorVariable* m_pSpecularColorVariable;
	bool m_bSpecularLevelTexture;
	static ID3DX11EffectScalarVariable* m_pUseSpecularLevelTextureVariable;
	TextureData* m_pSpecularLevelTexture = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pSpecularLevelSRVvariable;
	int m_Shininess;
	static ID3DX11EffectScalarVariable* m_pShininessVariable;

	//AMBIENT
	//*******
	XMFLOAT4 m_ColorAmbient;
	static ID3DX11EffectVectorVariable* m_pAmbientColorVariable;
	float m_AmbientIntensity;
	static ID3DX11EffectScalarVariable* m_pAmbientIntensityVariable;

	//NORMAL MAPPING
	//**************
	bool m_bFlipGreenChannel;
	static ID3DX11EffectScalarVariable* m_pFlipGreenChannelVariable;
	bool m_bNormalMapping;
	static ID3DX11EffectScalarVariable* m_pUseNormalMappingVariable;
	TextureData* m_pNormalMappingTexture = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pNormalMappingSRVvariable;
	
	//ENVIRONMENT MAPPING
	//*******************
	bool m_bEnvironmentMapping;
	static ID3DX11EffectScalarVariable* m_pUseEnvironmentMappingVariable;
	TextureData* m_pEnvironmentCube = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pEnvironmentSRVvariable;
	float m_ReflectionStrength;
	static ID3DX11EffectScalarVariable* m_pReflectionStrengthVariable;
	float m_RefractionStrength;
	static ID3DX11EffectScalarVariable* m_pRefractionStrengthVariable;
	float m_RefractionIndex;
	static ID3DX11EffectScalarVariable* m_pRefractionIndexVariable;

	//OPACITY
	//***************
	float m_Opacity;
	static ID3DX11EffectScalarVariable* m_pOpacityVariable;
	bool m_bOpacityMap;
	static ID3DX11EffectScalarVariable* m_pUseOpacityMapVariable;
	TextureData* m_pOpacityMap = nullptr;
	static ID3DX11EffectShaderResourceVariable* m_pOpacitySRVvariable;

	//SPECULAR MODELS
	//***************
	bool m_bSpecularBlinn;
	static ID3DX11EffectScalarVariable* m_pUseBlinnVariable;
	bool m_bSpecularPhong;
	static ID3DX11EffectScalarVariable* m_pUsePhongVariable;

	//FRESNEL FALLOFF
	//***************
	bool m_bFresnelFaloff;
	static ID3DX11EffectScalarVariable* m_pUseFresnelFalloffVariable;
	XMFLOAT4 m_ColorFresnel;
	static ID3DX11EffectVectorVariable* m_pFresnelColorVariable;
	float m_FresnelPower;
	static ID3DX11EffectScalarVariable* m_pFresnelPowerVariable;
	float m_FresnelMultiplier;
	static ID3DX11EffectScalarVariable* m_pFresnelMultiplierVariable;
	float m_FresnelHardness;
	static ID3DX11EffectScalarVariable* m_pFresnelHardnessVariable;
private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	UberMaterial(const UberMaterial &obj);
	UberMaterial& operator=(const UberMaterial& obj);
};

