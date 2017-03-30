//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CubeMapTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Content\ContentManager.h"
#include "Content\TextureDataLoader.h"
#include "Prefabs\SkyBoxPrefab.h"

#include "Graphics\TextRenderer.h"

CubeMapTestScene::CubeMapTestScene() :
	GameScene(L"CubeMapTestScene")
{
}

CubeMapTestScene::~CubeMapTestScene()
{
	TextRenderer::DestroyInstance();
}

void CubeMapTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_CubeMapCube = new SkyBoxPrefab(L"Resources/Textures/SkyBox.dds");

	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/Fonts/open_sans.fnt");

	AddChild(m_CubeMapCube);
}

void CubeMapTestScene::Update(const GameContext& gameContext)
{
	//float flickerRate = 10.0f;
	//if (((int)(gameContext.pGameTime->GetTotal() * flickerRate)) % 2 == 0)
	//{
	//	m_CubeMapCube->SetTextureFilepath(L"Resources/Textures/SkyBox.dds");
	//}
	//else
	//{
	//	m_CubeMapCube->SetTextureFilepath(L"Resources/Textures/Sunol_Cubemap.dds");
	//}
}

void CubeMapTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	const wchar_t* text = L"What a beautiful sky box";
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, text, XMFLOAT2(12, 12),(XMFLOAT4)Colors::Black);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, text, XMFLOAT2(10, 10), (XMFLOAT4)Colors::PeachPuff);
}
