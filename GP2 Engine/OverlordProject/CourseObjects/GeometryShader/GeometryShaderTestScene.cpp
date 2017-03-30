//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GeometryShaderTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Base\InputManager.h"
#include "Content\ContentManager.h"
#include "Content\TextureDataLoader.h"
#include "Graphics\TextRenderer.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"
#include "Components\RigidBodyComponent.h"
#include "Components\ColliderComponent.h"
#include "Components\TransformComponent.h"
#include "Components\ModelComponent.h"
#include "Components\TerrainComponent.h"
#include "Prefabs\SkyBoxPrefab.h"
#include "Prefabs\SpherePrefab.h"
#include "Materials\GrassMaterial.h"

GeometryShaderTestScene::GeometryShaderTestScene() :
	GameScene(L"GeometryShaderTestScene")
{
}

GeometryShaderTestScene::~GeometryShaderTestScene()
{
	TextRenderer::DestroyInstance();
}

void GeometryShaderTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto physx = PhysxManager::GetInstance()->GetPhysics();

	GetPhysxProxy()->EnablePhysxDebugRendering(false);

	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/Fonts/open_sans.fnt");

	m_CubeMapCube = new SkyBoxPrefab(L"Resources/Textures/SkyBox.dds");
	AddChild(m_CubeMapCube);

	// Terrain
	TerrainComponent* terrainComponent = new TerrainComponent(L"Resources/Terrain/Hawai_HeightMap_128x128x16.raw",
		L"Resources/Terrain/Hawai_TexMap.dds", 128, 128, 100.0f, 100.0f, 30.0f);

	m_TerrainObject = new GameObject();
	m_TerrainObject->AddComponent(terrainComponent);

	AddChild(m_TerrainObject);

	// Sphere
	float sphereRadius = 1.0f;
	m_pSphere = new SpherePrefab(sphereRadius, 10, (XMFLOAT4)Colors::BlueViolet);

	m_pSphereRBComponent = new RigidBodyComponent();
	m_pSphere->AddComponent(m_pSphereRBComponent);

	std::shared_ptr<PxGeometry> sphereGeometry(new PxSphereGeometry(sphereRadius));
	PxMaterial* defaultMaterial = physx->createMaterial(0.5f, 0.5f, 0.1f);
	ColliderComponent* sphereCollider = new ColliderComponent(sphereGeometry, *defaultMaterial);

	m_pSphere->AddComponent(sphereCollider);

	m_pSphere->GetTransform()->Translate(XMFLOAT3(0.0f, 100.0f, 0.0f));

	AddChild(m_pSphere);

	// Teapot
	auto grassMaterial = new GrassMaterial();
	grassMaterial->SetDiffuseTexture(L"./Resources/Textures/grass.tga");
	gameContext.pMaterialManager->AddMaterial(grassMaterial, 52);

	auto model = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	model->SetMaterial(52);

	auto teapot = new GameObject();
	teapot->AddComponent(model);
	AddChild(teapot);
	teapot->GetTransform()->Translate(0, 0, 20);

	// Input
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::UP), InputTriggerState::Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::LEFT), InputTriggerState::Down, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::DOWN), InputTriggerState::Down, VK_DOWN));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::RIGHT), InputTriggerState::Down, VK_RIGHT));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::JUMP), InputTriggerState::Pressed, VK_SPACE));
	gameContext.pInput->AddInputAction(InputAction(int(InputActions::RESET), InputTriggerState::Pressed, 'R'));
}

void GeometryShaderTestScene::Update(const GameContext& gameContext)
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
		sphereForce.x = -movementForce;
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
	if (gameContext.pInput->IsActionTriggered(int(InputActions::RESET)))
	{
		Reset();
	}

	if (!sphereForce.isZero())
	{
		m_pSphereRBComponent->AddForce(sphereForce * dt, forceMode);
	}
}

void GeometryShaderTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	const wchar_t* text = L"Mountain";
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, text, XMFLOAT2(5, 5), (XMFLOAT4)Colors::FloralWhite);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, text, XMFLOAT2(6, 6), (XMFLOAT4)Colors::Black);
}

void GeometryShaderTestScene::Reset()
{
	m_pSphereRBComponent->ClearForce();
	m_pSphereRBComponent->ClearTorque();
	m_pSphereRBComponent->GetPxRigidBody()->setLinearVelocity(PxVec3(0.0f));
	m_pSphereRBComponent->GetPxRigidBody()->setAngularVelocity(PxVec3(0.0f));
	m_pSphere->GetTransform()->Translate(XMFLOAT3(0.0f, 100.0f, 0.0f));
}
