#pragma once
#include "BaseComponent.h"

class ColliderComponent;

enum RigidBodyConstraintFlag
{
	RotX = (1 << 0),
	RotY = (1 << 1),
	RotZ = (1 << 2),
	TransX = (1 << 3),
	TransY = (1 << 4),
	TransZ = (1 << 5)
};

class RigidBodyComponent : public BaseComponent
{
public:
	RigidBodyComponent(bool isStatic = false);
	virtual ~RigidBodyComponent(void);

	virtual bool IsStatic() { return m_isStatic; };
	virtual bool IsDynamic() { return !m_isStatic; };
	virtual bool IsKinematic() { return m_isKinematic; };

	void SetKinematic(bool isKinematic); 
	void PutToSleep();

	PxRigidBody* GetPxRigidBody();
	void AddForce(const PxVec3& force, PxForceMode::Enum mode = PxForceMode::eFORCE, bool autowake = true);
	void AddTorque(const PxVec3& torque, PxForceMode::Enum mode = PxForceMode::eFORCE, bool autowake = true);
	void ClearForce(PxForceMode::Enum mode = PxForceMode::eFORCE);
	void ClearTorque(PxForceMode::Enum mode = PxForceMode::eFORCE);
	void SetConstraint(RigidBodyConstraintFlag flag, bool enable);
	void SetCollisionGroup(CollisionGroupFlag group);
	void SetCollisionIgnoreGroups(CollisionGroupFlag ignoreGroups);
	void SetDensity(float density);

	void UpdateColliderPoses();

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

	virtual void CreateActor();
	void AddCollider(ColliderComponent* collider); 

	void Translate(XMFLOAT3 position);
	void Rotate(XMFLOAT4 rotation);

	XMFLOAT3 GetPosition();
	XMFLOAT4 GetRotation();

	physx::PxRigidActor* m_pActor;
	vector<ColliderComponent*> m_Colliders;
	
	bool m_isStatic;
	bool m_isKinematic;

	PxD6Joint* m_pConstraintJoint;
	PxFilterData m_CollisionGroups;

private:

	UINT m_InitialConstraints;

private:

	friend class TransformComponent;
	friend class ColliderComponent;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	RigidBodyComponent( const RigidBodyComponent& obj);
	RigidBodyComponent& operator=( const RigidBodyComponent& obj);
};
