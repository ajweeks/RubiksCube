#pragma once
#include "Scenegraph/GameObject.h"

class ControllerComponent;
class CameraComponent;

class Character : public GameObject
{
public:
	enum class CharacterMovement
	{
		LEFT,
		RIGHT,
		FORWARD,
		BACKWARD,
		JUMP,
		CAMERA_SWAP
	};

	Character(float radius = 2, float height = 5, float moveSpeed = 100);
	virtual ~Character();
	virtual void Initialize(const GameContext& gameContext);
	virtual void PostInitialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);

	void SetFirstPerson(bool firstPerson);

protected:

	CameraComponent* m_pCamera;
	ControllerComponent* m_pController;

	float m_TotalPitch, m_TotalYaw;
	float m_MoveSpeed, m_RotationSpeed;
	float m_Radius, m_Height;

	bool m_FirstPerson;

	//Running
	float m_MaxRunVelocity, 
		m_TerminalVelocity,
		m_Gravity, 
		m_RunAccelerationTime, 
		m_JumpAccelerationTime, 
		m_RunAcceleration, 
		m_JumpAcceleration, 
		m_RunVelocity, 
		m_JumpVelocity;

	XMFLOAT3 m_Velocity;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Character(const Character& t);
	Character& operator=(const Character& t);
};


