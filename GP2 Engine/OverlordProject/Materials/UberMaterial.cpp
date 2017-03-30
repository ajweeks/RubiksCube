
#include "stdafx.h"

#include "Materials\UberMaterial.h"
#include "Content\ContentManager.h"
#include "Components\ModelComponent.h"
#include "Graphics\TextureData.h"

#pragma region Static variables
ID3DX11EffectVectorVariable* UberMaterial::m_pLightDirectionVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseDiffuseTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pDiffuseSRVvariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pDiffuseColorVariable = nullptr;

ID3DX11EffectVectorVariable* UberMaterial::m_pSpecularColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseSpecularLevelTextureVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pSpecularLevelSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pShininessVariable = nullptr;

ID3DX11EffectVectorVariable* UberMaterial::m_pAmbientColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pAmbientIntensityVariable = nullptr;

ID3DX11EffectScalarVariable* UberMaterial::m_pFlipGreenChannelVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseNormalMappingVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pNormalMappingSRVvariable = nullptr;

ID3DX11EffectScalarVariable* UberMaterial::m_pUseEnvironmentMappingVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pEnvironmentSRVvariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pReflectionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionStrengthVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionIndexVariable = nullptr;

ID3DX11EffectScalarVariable* UberMaterial::m_pOpacityVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseOpacityMapVariable = nullptr;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pOpacitySRVvariable = nullptr;

ID3DX11EffectScalarVariable* UberMaterial::m_pUseBlinnVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pUsePhongVariable = nullptr;

ID3DX11EffectScalarVariable* UberMaterial::m_pUseFresnelFalloffVariable = nullptr;
ID3DX11EffectVectorVariable* UberMaterial::m_pFresnelColorVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelPowerVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelMultiplierVariable = nullptr;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelHardnessVariable = nullptr;
#pragma endregion Static variables

UberMaterial::UberMaterial() :
	Material(L"Resources/Effects/Uber.fx", L"WithAlphaBlending")
{
}

UberMaterial::~UberMaterial()
{
}

#pragma region Getters and Setters
void UberMaterial::SetLightDirection(XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

void UberMaterial::EnableDiffuseTexture(bool enable)
{
	m_bDiffuseTexture = enable;
}

void UberMaterial::SetDiffuseTexture(const wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetDiffuseColor(XMFLOAT4 color)
{
	m_ColorDiffuse = color;
}

void UberMaterial::SetSpecularColor(XMFLOAT4 color)
{
	m_ColorSpecular = color;
}

void UberMaterial::EnableSpecularLevelTexture(bool enable)
{
	m_bSpecularLevelTexture = enable;
}

void UberMaterial::SetSpecularLevelTexture(const wstring& assetFile)
{
	m_pSpecularLevelTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetShininess(int shininess)
{
	m_Shininess = shininess;
}

void UberMaterial::SetAmbientColor(XMFLOAT4 color)
{
	m_ColorAmbient = color;
}

void UberMaterial::SetAmbientIntensity(float intensity)
{
	m_AmbientIntensity = intensity;
}

void UberMaterial::FlipNormalGreenCHannel(bool flip)
{
	m_bFlipGreenChannel = flip;
}

void UberMaterial::EnableNormalMapping(bool enable)
{
	m_bNormalMapping = enable;
}

void UberMaterial::SetNormalMapTexture(const wstring& assetFile)
{
	m_pNormalMappingTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableEnvironmentMapping(bool enable)
{
	m_bEnvironmentMapping = enable;
}

void UberMaterial::SetEnvironmentCube(const wstring& assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetReflectionStrength(float strength)
{
	m_ReflectionStrength = strength;
}

void UberMaterial::SetRefractionStrength(float strength)
{
	m_RefractionStrength = strength;
}

void UberMaterial::SetRefractionIndex(float index)
{
	m_RefractionIndex = index;
}

void UberMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}

void UberMaterial::EnableOpacityMap(bool enable)
{
	m_bOpacityMap = enable;
}

void UberMaterial::SetOpacityTexture(const wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::EnableSpecularBlinn(bool enable)
{
	m_bSpecularBlinn = enable;
}

void UberMaterial::EnableSpecularPhong(bool enable)
{
	m_bSpecularPhong = enable;
}

void UberMaterial::EnableFresnelFaloff(bool enable)
{
	m_bFresnelFaloff = enable;
}

void UberMaterial::SetFresnelColor(XMFLOAT4 color)
{
	m_ColorFresnel = color;
}

void UberMaterial::SetFresnelPower(float power)
{
	m_FresnelPower = power;
}

void UberMaterial::SetFresnelMultiplier(float multiplier)
{
	m_FresnelMultiplier = multiplier;
}

void UberMaterial::SetFresnelHardness(float hardness)
{
	m_FresnelHardness = hardness;
}
#pragma endregion Getters and Setters

void UberMaterial::LoadEffectVariables()
{
	m_pLightDirectionVariable = m_pEffect->GetVariableByName("gLightDirection")->AsVector();

	m_pUseDiffuseTextureVariable = m_pEffect->GetVariableByName("gUseTextureDiffuse")->AsScalar();
	m_pDiffuseSRVvariable = m_pEffect->GetVariableByName("gTextureDiffuse")->AsShaderResource();
	m_pDiffuseColorVariable = m_pEffect->GetVariableByName("gColorDiffuse")->AsVector();

	m_pSpecularColorVariable = m_pEffect->GetVariableByName("gColorSpecular")->AsVector();
	m_pUseSpecularLevelTextureVariable = m_pEffect->GetVariableByName("gUseTextureSpecularIntensity")->AsScalar();
	m_pSpecularLevelSRVvariable = m_pEffect->GetVariableByName("gTextureSpecularIntensity")->AsShaderResource();
	m_pShininessVariable = m_pEffect->GetVariableByName("gShininess")->AsScalar();

	m_pAmbientColorVariable = m_pEffect->GetVariableByName("gColorAmbient")->AsVector();
	m_pAmbientIntensityVariable = m_pEffect->GetVariableByName("gAmbientIntensity")->AsScalar();

	m_pFlipGreenChannelVariable = m_pEffect->GetVariableByName("gFlipGreenChannel")->AsScalar();
	m_pUseNormalMappingVariable = m_pEffect->GetVariableByName("gUseTextureNormal")->AsScalar();
	m_pNormalMappingSRVvariable = m_pEffect->GetVariableByName("gTextureNormal")->AsShaderResource();

	m_pUseEnvironmentMappingVariable = m_pEffect->GetVariableByName("gUseTextureEnvironment")->AsScalar();
	m_pEnvironmentSRVvariable = m_pEffect->GetVariableByName("gCubeEnvironment")->AsShaderResource();
	m_pReflectionStrengthVariable = m_pEffect->GetVariableByName("gReflectionStrength")->AsScalar();
	m_pRefractionStrengthVariable = m_pEffect->GetVariableByName("gRefractionStrength")->AsScalar();
	m_pRefractionIndexVariable = m_pEffect->GetVariableByName("gRefractionIndex")->AsScalar();

	m_pOpacityVariable = m_pEffect->GetVariableByName("gOpacityIntensity")->AsScalar();
	m_pUseOpacityMapVariable = m_pEffect->GetVariableByName("gUseTextureOpacity")->AsScalar();
	m_pOpacitySRVvariable = m_pEffect->GetVariableByName("gTextureOpacity")->AsShaderResource();

	m_pUseBlinnVariable = m_pEffect->GetVariableByName("gUseSpecularBlinn")->AsScalar();
	m_pUsePhongVariable = m_pEffect->GetVariableByName("gUseSpecularPhong")->AsScalar();

	m_pUseFresnelFalloffVariable = m_pEffect->GetVariableByName("gUseFresnelFalloff")->AsScalar();
	m_pFresnelColorVariable = m_pEffect->GetVariableByName("gColorFresnel")->AsVector();
	m_pFresnelPowerVariable = m_pEffect->GetVariableByName("gFresnelPower")->AsScalar();
	m_pFresnelMultiplierVariable = m_pEffect->GetVariableByName("gFresnelMultiplier")->AsScalar();
	m_pFresnelHardnessVariable = m_pEffect->GetVariableByName("gFresnelHardness")->AsScalar();
}

void UberMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	m_pLightDirectionVariable->SetFloatVector((float*)&m_LightDirection);

	m_pUseDiffuseTextureVariable->SetBool(m_bDiffuseTexture);
	if (m_bDiffuseTexture && m_pDiffuseTexture) m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());
	m_pDiffuseColorVariable->SetFloatVector((float*)&m_ColorDiffuse);
	
	m_pSpecularColorVariable->SetFloatVector((float*)&m_ColorSpecular);
	m_pUseSpecularLevelTextureVariable->SetBool(m_bSpecularLevelTexture);
	if (m_bSpecularLevelTexture && m_pSpecularLevelTexture) m_pSpecularLevelSRVvariable->SetResource(m_pSpecularLevelTexture->GetShaderResourceView());
	m_pShininessVariable->SetFloat((float)m_Shininess);
	
	m_pAmbientColorVariable->SetFloatVector((float*)&m_ColorAmbient);
	m_pAmbientIntensityVariable->SetFloat(m_AmbientIntensity);
	
	m_pFlipGreenChannelVariable->SetBool(m_bFlipGreenChannel);
	m_pUseNormalMappingVariable->SetBool(m_bNormalMapping);
	if (m_bNormalMapping && m_pNormalMappingTexture) m_pNormalMappingSRVvariable->SetResource(m_pNormalMappingTexture->GetShaderResourceView());

	m_pUseEnvironmentMappingVariable->SetBool(m_bEnvironmentMapping);
	if (m_bEnvironmentMapping && m_pEnvironmentCube) m_pEnvironmentSRVvariable->SetResource(m_pEnvironmentCube->GetShaderResourceView());
	m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);
	m_pRefractionStrengthVariable->SetFloat(m_RefractionStrength);
	m_pRefractionIndexVariable->SetFloat(m_RefractionIndex);

	m_pOpacityVariable->SetFloat(m_Opacity);
	m_pUseOpacityMapVariable->SetBool(m_bOpacityMap);
	if (m_bOpacityMap && m_pOpacityMap) m_pOpacitySRVvariable->SetResource(m_pOpacityMap->GetShaderResourceView());

	m_pUseBlinnVariable->SetBool(m_bSpecularBlinn);
	m_pUsePhongVariable->SetBool(m_bSpecularPhong);

	m_pUseFresnelFalloffVariable->SetBool(m_bFresnelFaloff);
	m_pFresnelColorVariable->SetFloatVector((float*)&m_ColorFresnel);
	m_pFresnelPowerVariable->SetFloat(m_FresnelPower);
	m_pFresnelMultiplierVariable->SetFloat(m_FresnelMultiplier);
	m_pFresnelHardnessVariable->SetFloat(m_FresnelHardness);
}
