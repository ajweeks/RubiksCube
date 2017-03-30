#pragma once
#include "../Scenegraph/GameObject.h"

struct GameContext;
class CameraComponent;

class FixedCamera : public GameObject
{
public:
	FixedCamera(const XMFLOAT3& offset = XMFLOAT3(0, 0, -10), const XMFLOAT3& rotation = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual ~FixedCamera();

	void SetOffset(const XMFLOAT3& offset);
	void SetRotation(float pitch, float yaw);

protected:
	virtual void Initialize(const GameContext& gameContext);

private:

	CameraComponent *m_pCamera;

	XMFLOAT3 m_Offset;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	FixedCamera(const FixedCamera& yRef);									
	FixedCamera& operator=(const FixedCamera& yRef);
};

