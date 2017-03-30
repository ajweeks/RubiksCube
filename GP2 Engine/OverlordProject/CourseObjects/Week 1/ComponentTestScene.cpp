//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ComponentTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"

#define FPS_COUNTER 1

ComponentTestScene::ComponentTestScene(void):
	GameScene(L"ComponentTestScene"),
	m_FpsInterval(FPS_COUNTER)
{
}


ComponentTestScene::~ComponentTestScene(void)
{
}

void ComponentTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	//GetPhysxProxy()->EnablePhysXFrameStepping(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	//PhysX
	auto physX = PhysxManager::GetInstance()->GetPhysics();	
	auto bouncyMaterial = physX->createMaterial(0.5, 0.5, 1.0f);

	//GROUND (Prefab?)
	//****************
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));
	
	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom,*bouncyMaterial,PxTransform(PxQuat(XM_PIDIV2,PxVec3(0,0,1)))));
	AddChild(ground);

	//SPHERE 1 (Group0) + (Ignore Group1 & Group2)
	//*********
	auto pSphere = new SpherePrefab();
	pSphere->GetTransform()->Translate(0, 30, 0);

	auto pRigidBody = new RigidBodyComponent();
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group0); //SET GROUP
	pRigidBody->SetCollisionIgnoreGroups(static_cast<CollisionGroupFlag>(Group1|Group2)); //IGNORE GROUP	
	pSphere->AddComponent(pRigidBody);

	std::shared_ptr<PxGeometry> spheregeom(new PxSphereGeometry(1));
	pSphere->AddComponent(new ColliderComponent(spheregeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(pSphere);

	//SPHERE 2 (Group1)
	pSphere = new SpherePrefab();
	pSphere->GetTransform()->Translate(0, 20, 0);
	
	pRigidBody = new RigidBodyComponent();
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group1); //SET GROUP
	pSphere->AddComponent(pRigidBody);

	pSphere->AddComponent(new ColliderComponent(spheregeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(pSphere);

	//SPHERE 3 (Group2)
	pSphere = new SpherePrefab();
	pSphere->GetTransform()->Translate(0, 10, 0);

	pRigidBody = new RigidBodyComponent();
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group2); //SET GROUP
	pSphere->AddComponent(pRigidBody);

	pSphere->AddComponent(new ColliderComponent(spheregeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(pSphere);
}

void ComponentTestScene::Update(const GameContext& gameContext)
{

	m_FpsInterval += gameContext.pGameTime->GetElapsed();
	if (m_FpsInterval >= FPS_COUNTER)
	{
		m_FpsInterval -= FPS_COUNTER;
		Logger::LogFormat(LogLevel::Info, L"FPS: %i", gameContext.pGameTime->GetFPS());
	}

}

void ComponentTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
