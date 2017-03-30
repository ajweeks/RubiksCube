//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ModelTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Physx\PhysxProxy.h"

ModelTestScene::ModelTestScene(void) :
	GameScene(L"ModelTestScene"),
	m_pChair(nullptr)
{
}

ModelTestScene::~ModelTestScene(void)
{
}

void ModelTestScene::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);

	//GROUND PLANE
	//************
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	//CHAIR OBJECT
	//************
	m_pChair = new GameObject();

	ColorMaterial* colorMaterial = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);

	ModelComponent* modelComponent = new ModelComponent(L"Resources/Meshes/Chair.ovm");
	//modelComponent->SetMaterial(0);

	m_pChair->AddComponent(modelComponent);

	DiffuseMaterial* diffuseMaterial = new DiffuseMaterial();
	diffuseMaterial->SetDiffuseTexture(L"Resources/Textures/Chair_Dark.dds");

	gameContext.pMaterialManager->AddMaterial(diffuseMaterial, 1);

	modelComponent->SetMaterial(1);

	RigidBodyComponent* rbComponent = new RigidBodyComponent();
	m_pChair->AddComponent(rbComponent);

	PxConvexMesh* chairMesh = ContentManager::Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");
	std::shared_ptr<PxGeometry> meshGeometry(new PxConvexMeshGeometry(chairMesh));
	PxMaterial* defaultMat = physX->createMaterial(0.5f, 0.5f, 0.1f);
	ColliderComponent* colliderComponent = new ColliderComponent(meshGeometry, *defaultMat);
	m_pChair->AddComponent(colliderComponent);

	AddChild(m_pChair);

	m_pChair->GetTransform()->Translate(0.0f, 12.0f, 0.0f);
}

void ModelTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	gameContext.pMaterialManager->GetMaterial(0)->SetEffectVariables(gameContext, m_pChair->GetComponent<ModelComponent>());
}

void ModelTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
