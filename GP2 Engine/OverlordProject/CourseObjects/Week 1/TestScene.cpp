//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TestScene.h"
#include <Prefabs/TorusPrefab.h>
#include <Components/Components.h>
#include <Physx/PhysxProxy.h>


TestScene::TestScene():
	GameScene(L"TestScene")
{
}


TestScene::~TestScene()
{
}

void TestScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Torus 0
	m_pTorus0 = new TorusPrefab(10.f,40,1.f,20, static_cast<XMFLOAT4>(Colors::Orange));
	AddChild(m_pTorus0);

	//Torus 1
	m_pTorus1 = new TorusPrefab(7.5f, 40, 1.f, 20, static_cast<XMFLOAT4>(Colors::Blue));
	m_pTorus0->AddChild(m_pTorus1);

	//Torus 2
	m_pTorus2 = new TorusPrefab(5.f, 40, 1.f, 20, static_cast<XMFLOAT4>(Colors::Red));
	m_pTorus1->AddChild(m_pTorus2);

	//Input
	gameContext.pInput->AddInputAction(InputAction(0, Down, VK_SPACE));
}

void TestScene::Update(const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(0))
	{
		m_TotalRotation += 60.f*gameContext.pGameTime->GetElapsed();

		m_pTorus0->GetTransform()->Rotate(0, m_TotalRotation, 0);
		m_pTorus1->GetTransform()->Rotate(m_TotalRotation, 0, 0);
		m_pTorus2->GetTransform()->Rotate(0, m_TotalRotation, 0);
	}
}

void TestScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void TestScene::SceneActivated()
{
}

void TestScene::SceneDeactivated()
{
}
