
#include "stdafx.h"

#include "BoneObject.h"
#include "..\Components\ModelComponent.h"
#include "..\Components\TransformComponent.h"

BoneObject::BoneObject(int boneID, int materialID, float length) :
	m_BoneID(boneID), m_MaterialID(materialID), m_Length(length)
{
}

BoneObject::~BoneObject()
{
}

void BoneObject::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	GameObject* boneObject = new GameObject();

	ModelComponent* modelComponent = new ModelComponent(L"Resources/Meshes/Bone.ovm");
	modelComponent->SetMaterial(m_MaterialID);
	boneObject->AddComponent(modelComponent);

	boneObject->GetTransform()->Rotate(0.0f, -90.0f, 0.0f);
	boneObject->GetTransform()->Scale(m_Length, m_Length, m_Length);

	AddChild(boneObject);
}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_Length, 0.0f, 0.0f);

	AddChild(pBone);
}

XMFLOAT4X4 BoneObject::GetBindPose() const
{
	return m_BindPose;
}

void BoneObject::CalculateBindPose()
{
	XMMATRIX worldV = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMStoreFloat4x4(&m_BindPose, XMMatrixInverse(nullptr, worldV));

	std::vector<BoneObject*> children = GetChildren<BoneObject>();
	for (auto it = children.begin(); it != children.end(); ++it)
	{
		(*it)->CalculateBindPose();
	}
}
