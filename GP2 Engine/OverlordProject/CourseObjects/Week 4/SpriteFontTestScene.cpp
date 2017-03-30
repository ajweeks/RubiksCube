//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SpriteFontTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Graphics\SpriteRenderer.h"
#include "Graphics\TextRenderer.h"
#include "Content\ContentManager.h"
#include "Graphics\TextureData.h"
#include "Components\SpriteComponent.h"
#include "Components\TransformComponent.h"
#include "Graphics\MeshFilter.h"

SpriteFontTestScene::SpriteFontTestScene() :
	GameScene(L"SpriteFontTestScene"),
	m_pSpriteFont(nullptr)
{
}

SpriteFontTestScene::~SpriteFontTestScene()
{
}

void SpriteFontTestScene::Initialize(const GameContext& gameContext)
{
	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/Fonts/open_sans.fnt");
}

void SpriteFontTestScene::Update(const GameContext& gameContext)
{
}

void SpriteFontTestScene::Draw(const GameContext& gameContext)
{
	const int lineCount = 30;
	const float lineHeight = 21.0f;
	for (size_t i = 0; i < lineCount; i++)
	{
		const int offset = (i + (int(gameContext.pGameTime->GetTotal() * 10.0f * (i / 5.0f)))) % lineCount;
		const XMFLOAT4 color = XMFLOAT4(
			(i / (float)lineCount), 
			1.0f - (i / (float)lineCount * 0.8f) + (sin(offset / (float)lineCount)) * 0.2f,
			0.5f + (i / (float)lineCount) / 2.0f, 
			1.0f);
		std::wstring origText = L"abcdefghijklmnopqrstuvwxyz1234567890+-";
		std::wstring text = origText.substr(offset) + origText.substr(0, offset);
		TextRenderer::GetInstance()->DrawText(m_pSpriteFont, text.c_str(), XMFLOAT2(10, 10 + i * lineHeight), color);
	}
}
