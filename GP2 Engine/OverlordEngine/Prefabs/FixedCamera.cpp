//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "FixedCamera.h"
#include "../Components/TransformComponent.h"

FixedCamera::FixedCamera(const XMFLOAT3& offset, const XMFLOAT3& rotation):
	m_pCamera(nullptr),
	m_Offset(offset)
{
	GetTransform()->Rotate(rotation);
}

FixedCamera::~FixedCamera()
{
}

void FixedCamera::SetOffset(const XMFLOAT3& offset)
{
	m_Offset = offset;
	GetTransform()->Translate(m_Offset);
}

void FixedCamera::SetRotation(float pitch, float yaw)
{
	GetTransform()->Rotate(pitch, yaw, 0.0f);
}

void FixedCamera::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//Camera Component
	m_pCamera = new CameraComponent();
	AddComponent(m_pCamera);
	
	GetTransform()->Translate(m_Offset);
}