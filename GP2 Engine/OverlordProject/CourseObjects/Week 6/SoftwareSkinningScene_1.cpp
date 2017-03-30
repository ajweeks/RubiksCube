//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CourseObjects\Week 6\SoftwareSkinningScene_1.h"
#include "Base\MaterialManager.h"
#include "Materials\ColorMaterial.h"
#include "Scenegraph\GameObject.h"
#include "Prefabs\BoneObject.h"
#include "Components\TransformComponent.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1() :
	GameScene(L"SoftwareSkinningScene_1"),
	m_pBone0(nullptr),
	m_pBone1(nullptr),
	m_RotationSign(1),
	m_BoneRotation(0.0f)
{
}

SoftwareSkinningScene_1::~SoftwareSkinningScene_1()
{
}

void SoftwareSkinningScene_1::Initialize(const GameContext& gameContext)
{
	ColorMaterial* colorMaterial = new ColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);
	
	GameObject* pRoot = new GameObject();

	m_pBone0 = new BoneObject(0, 0, 15.0f);
	pRoot->AddChild(m_pBone0);

	m_pBone1 = new BoneObject(1, 0, 15.0f);
	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);
}

void SoftwareSkinningScene_1::Update(const GameContext& gameContext)
{
	const float dt = gameContext.pGameTime->GetElapsed();
	const float maxRotation = XM_PIDIV4;
	const float rotationSpeed = 0.6f;

	m_BoneRotation += dt * m_RotationSign * rotationSpeed;

	if (m_RotationSign > 0 && m_BoneRotation > maxRotation)
	{
		m_RotationSign *= -1;
		m_BoneRotation = maxRotation;
	}
	else if (m_RotationSign < 0 && m_BoneRotation < -maxRotation)
	{
		m_RotationSign *= -1;
		m_BoneRotation = -maxRotation;
	}

	m_pBone0->GetTransform()->Rotate(0.0f, 0.0f, m_BoneRotation , false);
	m_pBone1->GetTransform()->Rotate(0.0f, 0.0f, -m_BoneRotation * 2.0f, false);
}

void SoftwareSkinningScene_1::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

}
