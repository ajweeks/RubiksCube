#include "stdafx.h"
#include "UberMaterialDefaultParams.h"


void SetDefaultParameters(UberMaterial* mat)
{
mat->EnableDiffuseTexture(false);
mat->SetDiffuseColor(XMFLOAT4(0.7f, 0.25f, 0.1f, 1.0f));

mat->SetAmbientColor(XMFLOAT4(0.179f, 0.12f, 0.21f, 1.0f));
mat->SetAmbientIntensity(0.05f);

mat->EnableSpecularPhong(true);
mat->EnableSpecularBlinn(false);
mat->EnableSpecularLevelTexture(false);
mat->SetShininess(30);
mat->SetSpecularColor(XMFLOAT4(0.95f, 0.95f, 1.0f, 1.0f));

mat->EnableEnvironmentMapping(true);
mat->SetEnvironmentCube(L"Resources/Textures/Sunol_Cubemap.dds");
mat->SetReflectionStrength(0.5f);

mat->EnableFresnelFaloff(true);
mat->SetFresnelColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
mat->SetFresnelPower(4.1f);
mat->SetFresnelHardness(3.5f);
mat->SetFresnelMultiplier(3.71f);

mat->EnableOpacityMap(false);
mat->SetOpacity(1.0f);

mat->SetLightDirection(XMFLOAT3(0.5f, 0.12f, 0.6f));

mat->EnableNormalMapping(false);
}
