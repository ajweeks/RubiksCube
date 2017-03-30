//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "PickTestScene.h"
#include "Scenegraph\GameObject.h"
#include "Diagnostics\Logger.h"
#include "Diagnostics\DebugRenderer.h"

#include "Prefabs\Prefabs.h"
#include "Components\Components.h"
#include "Physx\PhysxProxy.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"
#include "Graphics\MeshFilter.h"
#include "../../Materials/UberMaterial.h"
#include "CourseObjects\Week 3\UberMaterialDefaultParams.h"
#include "Components\ModelComponent.h"
#include "Graphics\TextRenderer.h"

PickTestScene::PickTestScene() :
	GameScene(L"PickTestScene")
{
}

PickTestScene::~PickTestScene()
{
}

void PickTestScene::Initialize(const GameContext& gameContext)
{
	GetPhysxProxy()->EnablePhysxDebugRendering(true);
	gameContext.pGameTime->ForceElapsedUpperbound(true);

	auto physX = PhysxManager::GetInstance()->GetPhysics();

	auto bouncyMaterial = physX->createMaterial(0, 0, 1);
	auto ground = new GameObject();
	ground->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<PxGeometry> geom(new PxPlaneGeometry());
	ground->AddComponent(new ColliderComponent(geom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(ground);

	auto pSphere = new SpherePrefab();
	pSphere->GetTransform()->Translate(0, 30, 0);

	auto pRigidBody = new RigidBodyComponent();
	pRigidBody->SetCollisionGroup(CollisionGroupFlag::Group0);
	pSphere->AddComponent(pRigidBody);

	std::shared_ptr<PxGeometry> spheregeom(new PxSphereGeometry(1));
	pSphere->AddComponent(new ColliderComponent(spheregeom, *bouncyMaterial, PxTransform(PxQuat(XM_PIDIV2, PxVec3(0, 0, 1)))));
	AddChild(pSphere);

	m_pSpriteFont = ContentManager::Load<SpriteFont>(L"Resources/Fonts/open_sans.fnt");

}

void PickTestScene::Update(const GameContext& gameContext)
{
	if (gameContext.pInput->IsMouseButtonDown(VK_RBUTTON))
	{
		GameObject* pickedObject = gameContext.pCamera->Pick(gameContext);
		if (pickedObject)
		{
			RigidBodyComponent* rbComponent = pickedObject->GetComponent<RigidBodyComponent>();

			if (rbComponent && !rbComponent->IsStatic())
			{
				float dPos = 5.0f;
				XMFLOAT3 previousTranslation = pickedObject->GetTransform()->GetPosition();
				pickedObject->GetTransform()->Translate(previousTranslation.x + dPos, previousTranslation.y, previousTranslation.z);
			}
		}
	}
}

void PickTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	TextRenderer::GetInstance()->DrawText(m_pSpriteFont, L"Right click on sphere to teleport it", XMFLOAT2(25, 25), 
		(XMFLOAT4)Colors::AntiqueWhite);
}
