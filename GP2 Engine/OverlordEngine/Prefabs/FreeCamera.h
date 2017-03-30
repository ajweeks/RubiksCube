#pragma once
#include "../Scenegraph/GameObject.h"

struct GameContext;
class CameraComponent;

class FreeCamera : public GameObject
{
public:
	FreeCamera(void);
	virtual ~FreeCamera(void);

	void SetRotation(float pitch, float yaw);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);

private:

	float m_TotalPitch, m_TotalYaw;
	float m_MoveSpeed, m_RotationSpeed, m_SpeedMultiplier;
	CameraComponent *m_pCamera;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	FreeCamera(const FreeCamera& yRef);									
	FreeCamera& operator=(const FreeCamera& yRef);
};

