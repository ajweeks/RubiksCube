//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CharacterTestScene.h"
#include "Scenegraph/GameObject.h"
#include "Components/Components.h"
#include "Content/ContentManager.h"
#include "Physx/PhysxManager.h"
#include "Materials/ColorMaterial.h"
#include "Materials/DiffuseMaterial.h"
#include "Physx\PhysxProxy.h"
#include "CourseObjects\Week 2\Character.h"

CharacterTestScene::CharacterTestScene(void) :
	GameScene(L"CharacterTestScene"),
	m_pCharacter(nullptr)
{
}

CharacterTestScene::~CharacterTestScene(void)
{

}

void CharacterTestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

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

	//CHARACTER
	//************
	m_pCharacter = new Character(2.0f, 5.0f, 0.8f);

	AddChild(m_pCharacter);
}

void CharacterTestScene::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CharacterTestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
