#include "stdafx.h"

#include "RubiksCubeScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Physx\PhysxProxy.h"
#include "Components\RigidBodyComponent.h"
#include "Prefabs\FixedCamera.h"
#include "Diagnostics\DebugRenderer.h"
#include "Graphics\TextRenderer.h"
#include "Content\ContentManager.h"

#include "RubiksCube.h"

RubiksCubeScene::RubiksCubeScene() :
	GameScene(L"RubiksCubeScene")
{
}

RubiksCubeScene::~RubiksCubeScene()
{
}

void RubiksCubeScene::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(false);
	DebugRenderer::EnableDebugRenderer(false);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	FixedCamera* fixedCamera = new FixedCamera(XMFLOAT3(0.5f, 1.5f, -3.0f), XMFLOAT3(18.0f, 0.0f, 0.0f));
	AddChild(fixedCamera);

	SetActiveCamera(fixedCamera->GetComponent<CameraComponent>());

	m_RubiksCube = new RubiksCube();

	AddChild(m_RubiksCube);

	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/Fonts/open_sans.fnt");
}

void RubiksCubeScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void RubiksCubeScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	float dY = 24.0f;
	float x = 10.0f;
	float y = 10.0f;
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Left click: Rotate clockwise", { x, y }); y += dY;
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Right click: Rotate anti-clockwise", { x, y }); y += dY;
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"S: Scramble", { x, y }); y += dY;
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"R: Reset", { x, y }); y += dY;
}
