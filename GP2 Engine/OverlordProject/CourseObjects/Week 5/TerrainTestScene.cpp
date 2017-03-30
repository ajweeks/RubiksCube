//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TerrainTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Content\ContentManager.h"
#include "Content\TextureDataLoader.h"
#include "Prefabs\SkyBoxPrefab.h"
#include "Components\TerrainComponent.h"
#include "Physx\PhysxProxy.h"
#include "Components\RigidBodyComponent.h"
#include "Components\ColliderComponent.h"
#include "Physx\PhysxManager.h"
#include "Base\InputManager.h"
#include "Components\TransformComponent.h"

#include "Graphics\TextRenderer.h"
#include "Prefabs\SpherePrefab.h"

TerrainTestScene::TerrainTestScene() :
	GameScene(L"TerrainTestScene")
{
}

TerrainTestScene::~TerrainTestScene()
{
	TextRenderer::DestroyInstance();
}

void TerrainTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	auto physx = PhysxManager::GetInstance()->GetPhysics();

	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	m_CubeMapCube = new SkyBoxPrefab(L"Resources/Textures/SkyBox.dds");

	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/Fonts/open_sans.fnt");

	AddChild(m_CubeMapCube);

	TerrainComponent* terrainComponent = new TerrainComponent(L"Resources/Terrain/Hawai_HeightMap_128x128x16.raw", 
		L"Resources/Terrain/Hawai_TexMap.dds", 128, 128, 100.0f, 100.0f, 30.0f);

	m_TerrainObject = new GameObject();
	m_TerrainObject->AddComponent(terrainComponent);

	AddChild(m_TerrainObject);

	float sphereRadius = 1.0f;
	m_pSphere = new SpherePrefab(sphereRadius, 10, (XMFLOAT4)Colors::BlueViolet);

	RigidBodyComponent* rbComponent = new RigidBodyComponent();
	m_pSphere->AddComponent(rbComponent);

	std::shared_ptr<PxGeometry> sphereGeometry(new PxSphereGeometry(sphereRadius));

	PxMaterial* defaultMaterial = physx->createMaterial(0.5f, 0.5f, 0.1f);
	ColliderComponent* sphereCollider = new ColliderComponent(sphereGeometry, *defaultMaterial);

	m_pSphere->AddComponent(sphereCollider);

	AddChild(m_pSphere);

	m_pSphere->GetTransform()->Translate(XMFLOAT3(0.0f, 100.0f, 0.0f));

	gameContext.pInput->AddInputAction(InputAction(int(InputActions::UP), InputTriggerState::Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::LEFT), InputTriggerState::Down, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::DOWN), InputTriggerState::Down, VK_DOWN));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::RIGHT), InputTriggerState::Down, VK_RIGHT));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::JUMP), InputTriggerState::Pressed, VK_SPACE));
}

void TerrainTestScene::Update(const GameContext& gameContext)
{
	const float dt = gameContext.pGameTime->GetElapsed();

	PxVec3 sphereForce = PxVec3(0.0f);

	const float movementForce = 3000.0f;
	const float jumpForce = 1500.0f;

	PxForceMode::Enum forceMode = PxForceMode::eFORCE;

	if (gameContext.pInput->IsActionTriggered(int(InputActions::UP)))
	{
		sphereForce.z = movementForce;
	}
	if (gameContext.pInput->IsActionTriggered(int(InputActions::DOWN)))
	{
		sphereForce.z = -movementForce;
	}
	if (gameContext.pInput->IsActionTriggered(int(InputActions::LEFT)))
	{
		sphereForce.x =- movementForce;
	}
	if (gameContext.pInput->IsActionTriggered(int(InputActions::RIGHT)))
	{
		sphereForce.x = movementForce;
	}	
	if (gameContext.pInput->IsActionTriggered(int(InputActions::JUMP)))
	{
		// Zero out other components to avoid jump because of Impuse force mdoe
		sphereForce = PxVec3(0.0f, jumpForce, 0.0f);
		forceMode = PxForceMode::eIMPULSE;
	}

	if (!sphereForce.isZero())
	{
		m_pSphere->GetComponent<RigidBodyComponent>()->AddForce(sphereForce * dt, forceMode);
	}
}

void TerrainTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	const wchar_t* text = L"Mountain";
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, text, XMFLOAT2(5, 5), (XMFLOAT4)Colors::FloralWhite);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, text, XMFLOAT2(6, 6), (XMFLOAT4)Colors::Black);
}
