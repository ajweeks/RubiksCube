//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "FreeCamera.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"

FreeCamera::FreeCamera(void):
	m_MoveSpeed(10),
	m_SpeedMultiplier(10.0f),
	m_RotationSpeed(90.f),
	m_TotalPitch(0),
	m_TotalYaw(0),
	m_pCamera(nullptr)
{
}


FreeCamera::~FreeCamera(void)
{
}

void FreeCamera::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Camera Component
	m_pCamera = new CameraComponent();
	AddComponent(m_pCamera);
}

void FreeCamera::SetRotation(float pitch, float yaw)
{
	m_TotalPitch = pitch;
	m_TotalYaw = yaw;
}

void FreeCamera::Update(const GameContext& gameContext)
{
	if(m_pCamera->IsActive())
	{
		//HANDLE INPUT
		XMFLOAT3 move = XMFLOAT3(0, 0, 0);
		move.z = gameContext.pInput->IsKeyboardKeyDown('W') ? 1.0f : 0.0f;
		if(move.z == 0) move.z = -(gameContext.pInput->IsKeyboardKeyDown('S') ? 1.0f : 0.0f);
		if(move.z == 0) move.z = gameContext.pInput->GetThumbstickPosition().y;

		move.x = gameContext.pInput->IsKeyboardKeyDown('D') ? 1.0f : 0.0f;
		if (move.x == 0) move.x = -(gameContext.pInput->IsKeyboardKeyDown('A') ? 1.0f : 0.0f);
		if (move.x == 0) move.x = gameContext.pInput->GetThumbstickPosition().x;

		move.y = gameContext.pInput->IsKeyboardKeyDown('E') ? 1.0f : 0.0f;
		if (move.y == 0) move.y = -(gameContext.pInput->IsKeyboardKeyDown('Q') ? 1.0f : 0.0f);

		auto currSpeed = m_MoveSpeed;
		if (gameContext.pInput->IsKeyboardKeyDown(VK_LSHIFT))
			currSpeed *= m_SpeedMultiplier;

		XMFLOAT2 look = XMFLOAT2(0,0);
		if(gameContext.pInput->IsMouseButtonDown(VK_LBUTTON))
		{
			auto mouseMove = gameContext.pInput->GetMouseMovement();
			look.x = static_cast<float>(mouseMove.x);
			look.y = static_cast<float>(mouseMove.y);
		}

		if(look.x == 0 && look.y == 0)
		{
			look = gameContext.pInput->GetThumbstickPosition(false);
		}

		//CALCULATE TRANSFORMS
		XMVECTOR forward = XMLoadFloat3(&GetTransform()->GetForward());
		XMVECTOR right = XMLoadFloat3(&GetTransform()->GetRight());
		XMVECTOR up = XMLoadFloat3(&GetTransform()->GetUp());
		XMVECTOR currPos = XMLoadFloat3(&GetTransform()->GetPosition());
		//XMVECTOR currRot = XMLoadFloat4(&GetTransform()->GetRotation());
		//XMVECTOR finalQuat;

		currPos += right * move.x * currSpeed * gameContext.pGameTime->GetElapsed();
		currPos += up * move.y * currSpeed * gameContext.pGameTime->GetElapsed();
		currPos += forward * move.z * currSpeed * gameContext.pGameTime->GetElapsed();
		
		m_TotalYaw += look.x * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
		m_TotalPitch += look.y * m_RotationSpeed * gameContext.pGameTime->GetElapsed();
		
		GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);
		GetTransform()->Translate(currPos);
	}
}
