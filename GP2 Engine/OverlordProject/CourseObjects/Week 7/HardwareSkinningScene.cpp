//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "HardwareSkinningScene.h"
#include "Scenegraph\GameObject.h"

#include "Content\ContentManager.h"
#include "Components\TransformComponent.h"
#include "Components\ModelComponent.h"
#include "Graphics\ModelAnimator.h"
#include "Materials\SkinnedColorMaterial.h"

#include "..\..\Materials\SkinnedDiffuseMaterial.h"

HardwareSkinningScene::HardwareSkinningScene() :
	GameScene(L"HardwareSkinningScene"),
	m_pKnightModel(nullptr),
	m_pBikeModel(nullptr)
{
}

HardwareSkinningScene::~HardwareSkinningScene()
{
}

void HardwareSkinningScene::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/Knight.jpg");
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 0);

	auto colorMaterial = new SkinnedColorMaterial();
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 1);


	m_pKnightModel = new ModelComponent(L"./Resources/Meshes/Knight.ovm");
	m_pKnightModel->SetMaterial(0);
	auto knightChildObject = new GameObject();
	knightChildObject->AddComponent(m_pKnightModel);
	AddChild(knightChildObject);

	m_pBikeModel = new ModelComponent(L"./Resources/Meshes/bike.ovm");
	m_pBikeModel->SetMaterial(1);
	auto bikeChildObject = new GameObject();
	bikeChildObject->AddComponent(m_pBikeModel);
	AddChild(bikeChildObject);

	bikeChildObject->GetTransform()->Translate(-30.0f, 0.0f, 0.0f);

	m_pKnightModel->GetAnimator()->SetAnimation(0);
	m_pBikeModel->GetAnimator()->SetAnimation(0);

	knightChildObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	bikeChildObject->GetTransform()->Scale(0.1f, 0.1f, 0.1f);

	//Input
	gameContext.pInput->AddInputAction(InputAction(0, Released, 'R'));
	gameContext.pInput->AddInputAction(InputAction(1, Released, 'P'));
	gameContext.pInput->AddInputAction(InputAction(2, Released, 'O'));

	gameContext.pInput->AddInputAction(InputAction(3, Down, VK_UP));
	gameContext.pInput->AddInputAction(InputAction(4, Down, VK_DOWN));

	gameContext.pInput->AddInputAction(InputAction(5, Pressed, VK_LEFT));
	gameContext.pInput->AddInputAction(InputAction(6, Pressed, VK_RIGHT));
}

void HardwareSkinningScene::Update(const GameContext& gameContext)
{
	auto knightAnimator = m_pKnightModel->GetAnimator();
	auto bikeAnimator = m_pBikeModel->GetAnimator();

	UpdateAnimator(knightAnimator, gameContext);
	UpdateAnimator(bikeAnimator, gameContext);
}

void HardwareSkinningScene::UpdateAnimator(ModelAnimator* animator, const GameContext& gameContext)
{
	if (gameContext.pInput->IsActionTriggered(0))
	{
		animator->Reset();
	}

	if (gameContext.pInput->IsActionTriggered(1))
	{
		if (animator->IsPlaying())
		{
			animator->Pause();
		}
		else
		{
			animator->Play();
		}
	}

	if (gameContext.pInput->IsActionTriggered(2))
	{
		animator->SetPlayReversed(!animator->IsReversed());
	}

	if (gameContext.pInput->IsActionTriggered(3))
	{
		auto speed = animator->GetAnimationSpeed();
		speed += 0.5f * gameContext.pGameTime->GetElapsed();
		Clamp<float>(speed, 5, 0);

		animator->SetAnimationSpeed(speed);
	}

	if (gameContext.pInput->IsActionTriggered(4))
	{
		auto speed = animator->GetAnimationSpeed();
		speed -= 0.5f * gameContext.pGameTime->GetElapsed();
		Clamp<float>(speed, 5, 0);

		animator->SetAnimationSpeed(speed);
	}

	if (gameContext.pInput->IsActionTriggered(5))
	{
		animator->SetAnimation(0);
	}

	if (gameContext.pInput->IsActionTriggered(6))
	{
		animator->SetAnimation(1);
	}
}

void HardwareSkinningScene::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}
