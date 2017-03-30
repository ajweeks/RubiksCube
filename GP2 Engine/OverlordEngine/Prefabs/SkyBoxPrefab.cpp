//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SkyBoxPrefab.h"
#include "../Components/Components.h"
#include "../../OverlordProject/Materials/CubeMapMaterial.h"
#include "../Graphics/TextureData.h"

SkyBoxPrefab::SkyBoxPrefab(const wstring& textureFilepath) :
	m_TextureFilepath(textureFilepath)
{
}


SkyBoxPrefab::~SkyBoxPrefab()
{
}

void SkyBoxPrefab::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_CubeMapMaterial = new CubeMapMaterial(m_TextureFilepath);

	gameContext.pMaterialManager->AddMaterial(m_CubeMapMaterial, 0);

	ModelComponent* modelComponent = new ModelComponent(L"Resources/Meshes/Box.ovm");
	modelComponent->SetMaterial(0);

	AddComponent(modelComponent);
}

void SkyBoxPrefab::SetTextureFilepath(const wstring& textureFilepath)
{
	m_TextureFilepath = textureFilepath;
	m_CubeMapMaterial->SetTexture(textureFilepath);
}
