//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CourseObjects\Week 3\UberMaterialTestScene.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Materials\UberMaterial.h"
#include "Scenegraph\GameObject.h"
#include "Components\RigidBodyComponent.h"
#include "Components\ColliderComponent.h"
#include "Components\ModelComponent.h"
#include "Components\TransformComponent.h"
#include "Content\ContentManager.h"
#include "CourseObjects\Week 3\UberMaterialDefaultParams.h"

UberMaterialTestScene::UberMaterialTestScene(void) :
	GameScene(L"UberMaterialTestScene")
{
}

UberMaterialTestScene::~UberMaterialTestScene(void)
{
}

void UberMaterialTestScene::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	//GROUND PLANE
	//************
	auto ground = new GameObject();
	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	AddChild(ground);

	//TEAPOT OBJECT
	//************
	m_pTeapot = new GameObject();

	UberMaterial* uberMaterial = new UberMaterial();
	SetDefaultParameters(uberMaterial);

	gameContext.pMaterialManager->AddMaterial(uberMaterial, 0);

	ModelComponent* modelComponent = new ModelComponent(L"Resources/Meshes/Teapot.ovm");
	modelComponent->SetMaterial(0);

	m_pTeapot->AddComponent(modelComponent);

	AddChild(m_pTeapot);
	
	//m_pTeapot->GetTransform()->Translate(0.0f, 12.0f, 0.0f);
}

void UberMaterialTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void UberMaterialTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
