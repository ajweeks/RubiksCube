//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "../Base/GeneralStructs.h"
#include "../Diagnostics/Logger.h"
#include "../Scenegraph/GameObject.h"

ColliderComponent::ColliderComponent(std::shared_ptr<PxGeometry> & geometry, const PxMaterial& material, const PxTransform& localPose)
	:
	m_Geometry(geometry),
	m_Material(material),
	m_LocalPose(localPose),
	m_pShape(nullptr),
	m_isTrigger(false)
{
}

ColliderComponent::~ColliderComponent()
{
}

void ColliderComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	auto rigidBody = GetGameObject()->GetComponent<RigidBodyComponent>();
		
	if (rigidBody == nullptr)
	{
		Logger::LogError(L"[ColliderComponent] Cannot add a Collider to an object that does not have a rigid body");
		return;
	}

	rigidBody->AddCollider(this);
}

void ColliderComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void ColliderComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

PxTransform ColliderComponent::GetLocalPose() const
{
	return m_LocalPose;
}

void ColliderComponent::SetLocalPose(const PxTransform& localPose)
{
	m_LocalPose = localPose;
}

void ColliderComponent::EnableTrigger(bool isTrigger) 
{ 
	m_isTrigger = isTrigger; 
	if(m_pShape != nullptr)
	{
		m_pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !isTrigger);
		m_pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, isTrigger);
	}
}

void ColliderComponent::SetShape(PxShape* shape) 
{ 
	m_pShape = shape; 
	if(m_isTrigger)
	{
		m_pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_isTrigger);
		m_pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_isTrigger);
	}
}