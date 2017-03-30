#pragma once
#include "BaseComponent.h"

class TransformComponent: public BaseComponent
{
public:
	TransformComponent(void);
	virtual ~TransformComponent(void);

	void Translate(float x, float y, float z);
	void Translate(const XMFLOAT3& position);
	void Translate(const XMVECTOR& position);

	void Rotate(float x, float y, float z, bool isEuler = true);
	void Rotate(const XMFLOAT3& rotation, bool isEuler = true);
	void Rotate(const XMVECTOR& rotation, bool isQuaternion = true);
	void Rotate(const XMFLOAT3& offset, const XMFLOAT3& deltaRotation);

	void Scale(float x, float y, float z);
	void Scale(const XMFLOAT3& scale);

	void SetStoreRotationMatrix(bool store);

	const XMFLOAT3& GetPosition() const { return m_Position; }
	const XMFLOAT3& GetWorldPosition() const { return m_WorldPosition; }
	const XMFLOAT3& GetScale() const { return m_Scale; }
	const XMFLOAT3& GetWorldScale() const { return m_WorldScale; }
	const XMFLOAT4& GetRotation() const { return m_Rotation; }
	const XMFLOAT3& GetRotationEuler() const;
	const XMFLOAT4& GetWorldRotation() const { return m_WorldRotation; }
	const XMFLOAT4X4& GetWorld() const { return m_World; }

	const XMFLOAT3& GetForward() const { return m_Forward; }
	const XMFLOAT3& GetUp() const { return m_Up; }
	const XMFLOAT3& GetRight() const { return m_Right; }

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);


	void UpdateTransforms(const GameContext& gameContext);
	bool CheckConstraints() const;

private:
	enum TransformChanged{
		NONE = 0x00,
		TRANSLATION = 0x01,
		ROTATION = 0x02,
		SCALE = 0x04,
	};

	unsigned char m_IsTransformChanged;

	XMFLOAT3 m_Position, m_WorldPosition, m_Scale, m_WorldScale, m_Forward, m_Up, m_Right;
	XMFLOAT4 m_Rotation, m_WorldRotation;
	XMFLOAT4X4 m_World;
	
	bool m_StoreRotationMatrix = false;
	XMFLOAT4X4 m_RotationMatrix;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TransformComponent( const TransformComponent& obj);
	TransformComponent& operator=( const TransformComponent& obj);
};
