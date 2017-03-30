//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Character.h"
#include "Components\Components.h"
#include "Prefabs\Prefabs.h"
#include "Physx\PhysxManager.h"
#include "Content\ContentManager.h"

Character::Character(float radius, float height, float moveSpeed) :
	m_Radius(radius),
	m_Height(height),
	m_MoveSpeed(moveSpeed),
	m_pCamera(nullptr),
	m_pController(nullptr),
	m_TotalPitch(0),
	m_TotalYaw(0),
	m_RotationSpeed(90.f),
	//Running
	m_MaxRunVelocity(50.0f),
	m_TerminalVelocity(20),
	m_Gravity(9.81f),
	m_RunAccelerationTime(0.3f),
	m_JumpAccelerationTime(0.15f),
	m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime),
	m_JumpAcceleration(m_Gravity / m_JumpAccelerationTime),
	m_RunVelocity(0),
	m_JumpVelocity(0),
	m_Velocity(0, 0, 0)
{
}

Character::~Character(void)
{
}

void Character::Initialize(const GameContext& gameContext)
{
	auto physx = PhysxManager::GetInstance()->GetPhysics();

	PxMaterial* defaultMaterial = physx->createMaterial(0.5f, 0.5f, 0.1f);
	
	// Create controller
	m_pController = new ControllerComponent(defaultMaterial, m_Radius, m_Height);
	AddComponent(m_pController);

	// Add a fixed camera as child
	FixedCamera* fixedCamera = new FixedCamera();
	AddChild(fixedCamera);

	// Register all Input Actions
	gameContext.pInput->AddInputAction(InputAction(int(CharacterMovement::FORWARD), InputTriggerState::Down, 'W'));
	gameContext.pInput->AddInputAction(InputAction(int(CharacterMovement::LEFT), InputTriggerState::Down, 'A'));
	gameContext.pInput->AddInputAction(InputAction(int(CharacterMovement::BACKWARD), InputTriggerState::Down, 'S'));
	gameContext.pInput->AddInputAction(InputAction(int(CharacterMovement::RIGHT), InputTriggerState::Down, 'D'));
	gameContext.pInput->AddInputAction(InputAction(int(CharacterMovement::JUMP), InputTriggerState::Pressed, VK_SPACE));
	gameContext.pInput->AddInputAction(InputAction(int(CharacterMovement::CAMERA_SWAP), InputTriggerState::Pressed, 'C'));

	Logger::LogInfo(L"Character initialized! Press C to swap cameras");
}

void Character::PostInitialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	FixedCamera* fixedCamera = GetChild<FixedCamera>();
	if (!fixedCamera)
	{
		Logger::LogError(L"Fixed camera was not added to character in intialization!");
		return;
	}
	// Set the camera as active
	// We need to do this in the PostInitialize because child game objects only get initialized after the Initialize of the current object finishes
	m_pCamera = fixedCamera->GetComponent<CameraComponent>();
	assert (m_pCamera != nullptr);
	m_pCamera->SetActive();

	// Get out of the ground
	m_pController->Translate(0.0f, m_Height / 2.0f + m_Radius + 0.1f, 0.0f);
	fixedCamera->SetRotation(14.0f, 0.0f);
	SetFirstPerson(false);
}

void Character::Update(const GameContext& gameContext)
{
	const float dt = gameContext.pGameTime->GetElapsed();
	InputManager* input = gameContext.pInput;

	// ---Rotation---
	XMFLOAT2 look = XMFLOAT2(0, 0);
	if (input->IsMouseButtonDown(VK_LBUTTON))
	{
		auto mouseMove = input->GetMouseMovement();
		look.x = static_cast<float>(mouseMove.x);
		look.y = static_cast<float>(mouseMove.y);
	}

	if (look.x == 0 && look.y == 0)
	{
		look = input->GetThumbstickPosition(false);
	}

	m_TotalYaw += look.x * m_RotationSpeed * dt;
	m_TotalPitch += look.y * m_RotationSpeed * dt;
	
	GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);

	// ---Translation---
	// Input
	XMFLOAT2 inputMovement = XMFLOAT2(0.0f, 0.0f);

	if (input->IsActionTriggered(int(CharacterMovement::LEFT)))
	{
		inputMovement.x = -1.0f;
	}
	if (input->IsActionTriggered(int(CharacterMovement::RIGHT)))
	{
		inputMovement.x = 1.0f;
	}
	if (input->IsActionTriggered(int(CharacterMovement::FORWARD)))
	{
		inputMovement.y = 1.0f;
	}
	if (input->IsActionTriggered(int(CharacterMovement::BACKWARD)))
	{
		inputMovement.y = -1.0f;
	}	
	
	TransformComponent* const transform = GetTransform();
	XMVECTOR forward = XMLoadFloat3(&transform->GetForward());
	XMVECTOR right = XMLoadFloat3(&transform->GetRight());

	XMVECTOR movementVec = XMVectorZero();
	movementVec += forward * inputMovement.y * m_MoveSpeed;
	movementVec += right * inputMovement.x * m_MoveSpeed;

	if (XMVectorGetX(movementVec) != 0.0f || XMVectorGetZ(movementVec) != 0.0f)
	{
		m_RunVelocity = m_RunAcceleration * dt;
		m_RunVelocity = max(m_RunVelocity, m_MaxRunVelocity);

		const float prevVelY = m_Velocity.y;

		XMVECTOR newVel = m_RunVelocity * movementVec;
		XMStoreFloat3(&m_Velocity, newVel);
		m_Velocity.y = prevVelY;
	}
	else
	{
		m_Velocity.x = 0.0f;
		m_Velocity.z = 0.0f;
	}

	// Gravity
	if (!(m_pController->GetCollisionFlags() & PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		// We aren't grounded, apply gravity
		m_JumpVelocity -= m_JumpAcceleration * dt;
		if (m_JumpVelocity < -m_TerminalVelocity) m_JumpVelocity = -m_TerminalVelocity;
	}
	else if(input->IsActionTriggered(int(CharacterMovement::JUMP)))
	{
		// We are grounded, check for jump inputs
		m_JumpVelocity = 0.0f;
		m_Velocity.y = 200.0f;
	}
	else
	{
		m_Velocity.y = 0.0f;
	}

	m_Velocity.y += m_JumpVelocity;

	XMFLOAT3 dtVel = XMFLOAT3(m_Velocity.x * dt, m_Velocity.y * dt, m_Velocity.z * dt);
	m_pController->Move(dtVel);

	if (input->IsActionTriggered(int(CharacterMovement::CAMERA_SWAP)))
	{
		SetFirstPerson(!m_FirstPerson);
	}
}

void Character::SetFirstPerson(bool firstPerson)
{
	m_FirstPerson = firstPerson;
	FixedCamera* fixedCamera = static_cast<FixedCamera*>(m_pCamera->GetGameObject());
	if (fixedCamera)
	{
		if (m_FirstPerson)
		{
			fixedCamera->SetOffset(XMFLOAT3(0, 0, 0));
		}
		else
		{
			fixedCamera->SetOffset(XMFLOAT3(0, 8, -20));
		}
	}
}
