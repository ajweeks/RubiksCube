//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SpikeyScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"
#include "Graphics\MeshFilter.h"
#include "../../Materials/SpikeyMaterial.h"
#include "Components\ModelComponent.h"

#define FPS_COUNTER 1

SpikeyScene::SpikeyScene():
	GameScene(L"SpikeyScene"),
	m_FpsInterval(FPS_COUNTER)
{
}

SpikeyScene::~SpikeyScene()
{
}

void SpikeyScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	// Create PhysX ground plane
	auto physX = PhysxManager::GetInstance()->GetPhysics();
	
	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*bouncyMaterial,PxTransform(PxQuat(XM_PIDIV2,PxVec3(0,0,1)))));
	AddChild(ground);

	auto myMaterial = new SpikeyMaterial();
	myMaterial->SetDiffuseTexture(L"./Resources/Textures/Chair_Dark.dds");
	gameContext.pMaterialManager->AddMaterial(myMaterial, 60);
	
	auto model = new ModelComponent(L"./Resources/Meshes/Teapot.ovm");
	model->SetMaterial(60);

	auto teapot = new GameObject();
	teapot->AddComponent(model);
	AddChild(teapot);
	teapot->GetTransform()->Translate(0,0,20);
}

void SpikeyScene::Update(const GameContext& gameContext)
{
	m_FpsInterval += gameContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	}
}

void SpikeyScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
