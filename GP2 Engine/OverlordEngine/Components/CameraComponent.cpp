//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CameraComponent.h"
#include "../Base/OverlordGame.h"
#include "../Components/TransformComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "../Base/GeneralStructs.h"


CameraComponent::CameraComponent(void) :
	m_FOV(XM_PIDIV4),
	m_NearPlane(0.1f),
	m_FarPlane(2500.0f),
	m_Size(25.0f),
	m_PerspectiveProjection(true),
	m_IsActive(true)
{
	XMStoreFloat4x4(&m_Projection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, XMMatrixIdentity());
}


CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CameraComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	auto windowSettings = OverlordGame::GetGameSettings().Window;
	XMMATRIX projection, view, viewInv, viewProjectionInv;

	if (m_PerspectiveProjection)
	{
		projection = XMMatrixPerspectiveFovLH(m_FOV, windowSettings.AspectRatio, m_NearPlane, m_FarPlane);
	}
	else
	{
		float viewWidth = (m_Size>0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
		float viewHeight = (m_Size>0) ? m_Size : windowSettings.Height;
		projection = XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	view = XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	viewInv = XMMatrixInverse(nullptr, view);
	viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void CameraComponent::SetActive()
{
	auto gameObject = GetGameObject();
	if (gameObject == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game object is null");
		return;
	}

	auto gameScene = gameObject->GetScene();
	if (gameScene == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game scene is null");
		return;
	}

	gameScene->SetActiveCamera(this);
}

GameObject* CameraComponent::Pick(const GameContext& gameContext, CollisionGroupFlag ignoreGroups) const
{
	auto gameObject = GetGameObject();
	if (gameObject == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to Pick. Parent game object is null");
		return nullptr;
	}

	auto gameScene = gameObject->GetScene();
	if (gameScene == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to Pick. Parent game scene is null");
		return nullptr;
	}

	auto physXProxy = gameScene->GetPhysxProxy();
	if (physXProxy == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to Pick. PhysX Proxy is null");
		return nullptr;
	}

	XMFLOAT3 nearPoint, farPoint;
	XMFLOAT2 ndc;
	PxVec3 rayStart, rayDir;

	auto windowSettings = OverlordGame::GetGameSettings().Window;
	auto mousePosition = gameContext.pInput->GetMousePosition();

	int halfWidth = windowSettings.Width / 2;
	int halfHeight = windowSettings.Height / 2;

	ndc.x = (static_cast<float>(mousePosition.x) - halfWidth) / halfWidth;
	ndc.y = (halfHeight - static_cast<float>(mousePosition.y)) / halfHeight;

	XMStoreFloat3(&nearPoint, XMVector3TransformCoord(XMVectorSet(ndc.x, ndc.y, 0, 0), XMLoadFloat4x4(&m_ViewProjectionInverse)));
	XMStoreFloat3(&farPoint, XMVector3TransformCoord(XMVectorSet(ndc.x, ndc.y, 1, 0), XMLoadFloat4x4(&m_ViewProjectionInverse)));

	rayStart = PxVec3(nearPoint.x, nearPoint.y, nearPoint.z);

	auto xmDirection = XMLoadFloat3(&farPoint) - XMLoadFloat3(&nearPoint);

	rayDir = PxVec3(XMVectorGetX(xmDirection), XMVectorGetY(xmDirection), XMVectorGetZ(xmDirection));
	rayDir.normalize();

	PxQueryFilterData filterData;
	filterData.data.word0 = ~ignoreGroups;

	PxRaycastBuffer hit;
	if (physXProxy->Raycast(rayStart, rayDir, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filterData))
	{
		auto actor = hit.block.actor;
		if (actor != nullptr)
		{
			auto userData = actor->userData;
			if (userData != nullptr)
			{
				return reinterpret_cast<BaseComponent*>(userData)->GetGameObject();
			}
		}
	}
	return nullptr;
}