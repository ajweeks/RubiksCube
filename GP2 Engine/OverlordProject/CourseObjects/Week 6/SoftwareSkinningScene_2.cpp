//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "CourseObjects\Week 6\SoftwareSkinningScene_2.h"
#include "Base\MaterialManager.h"
#include "Materials\ColorMaterial.h"
#include "Scenegraph\GameObject.h"
#include "Prefabs\BoneObject.h"
#include "Components\TransformComponent.h"
#include "MeshHelper.h"

SoftwareSkinningScene_2::SoftwareSkinningScene_2() :
	GameScene(L"SoftwareSkinningScene_2"),
	m_pBone0(nullptr),
	m_pBone1(nullptr),
	m_RotationSign(1),
	m_BoneRotation(0.0f)
{
}

SoftwareSkinningScene_2::~SoftwareSkinningScene_2()
{
}

void SoftwareSkinningScene_2::Initialize(const GameContext& gameContext)
{
	ColorMaterial* colorMaterial = new ColorMaterial(true);
	gameContext.pMaterialManager->AddMaterial(colorMaterial, 0);

	const float boneLength = 15.0f;

	GameObject* pRoot = new GameObject();

	m_pBone0 = new BoneObject(0, 0, boneLength);
	pRoot->AddChild(m_pBone0);

	m_pBone1 = new BoneObject(1, 0, boneLength);
	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);

	GameObject* meshDrawComponentHolder = new GameObject();
	m_pMeshDrawComponent = new MeshDrawComponent(24);
	meshDrawComponentHolder->AddComponent(m_pMeshDrawComponent);

	AddChild(meshDrawComponentHolder);

	CreateMesh(boneLength);

	m_pBone0->CalculateBindPose();
}

void SoftwareSkinningScene_2::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	// Rotation
	const float dt = gameContext.pGameTime->GetElapsed();
	const float maxRotation = XM_PIDIV4;
	const float rotationSpeed = 0.6f;
	
	m_BoneRotation += dt * m_RotationSign * rotationSpeed;
	
	if (m_RotationSign > 0 && m_BoneRotation > maxRotation)
	{
		m_RotationSign *= -1;
		m_BoneRotation = maxRotation;
	}
	else if (m_RotationSign < 0 && m_BoneRotation < -maxRotation)
	{
		m_RotationSign *= -1;
		m_BoneRotation = -maxRotation;
	}
	
	m_pBone0->GetTransform()->Rotate(0.0f, 0.0f, m_BoneRotation, false);
	m_pBone1->GetTransform()->Rotate(0.0f, 0.0f, -m_BoneRotation * 2.0f, false);

	// Skinning
	XMFLOAT4X4 bone0bindPose = m_pBone0->GetBindPose();
	XMFLOAT4X4 bone0World = m_pBone0->GetTransform()->GetWorld();
	XMMATRIX bone0Transform = XMLoadFloat4x4(&bone0bindPose) * XMLoadFloat4x4(&bone0World);

	XMFLOAT4X4 bone1bindPose = m_pBone1->GetBindPose();
	XMFLOAT4X4 bone1World = m_pBone1->GetTransform()->GetWorld();
	XMMATRIX bone1Transform = XMLoadFloat4x4(&bone1bindPose) * XMLoadFloat4x4(&bone1World);

	for (size_t i = 0; i < 24; i++)
	{
		XMVECTOR newPos = XMVector3Transform(XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position), bone0Transform);
		XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, newPos);
	}

	for (size_t i = 24; i < m_SkinnedVertices.size(); i++)
	{
		XMVECTOR newPos = XMVector3Transform(XMLoadFloat3(&m_SkinnedVertices[i].OriginalVertex.Position), bone1Transform);
		XMStoreFloat3(&m_SkinnedVertices[i].TransformedVertex.Position, newPos);
	}

	m_pMeshDrawComponent->RemoveTriangles();
	for (size_t i = 0; i < m_SkinnedVertices.size(); i += 4)
	{
		QuadPosNormCol quad(m_SkinnedVertices[i].TransformedVertex,
			m_SkinnedVertices[i + 1].TransformedVertex,
			m_SkinnedVertices[i + 2].TransformedVertex,
			m_SkinnedVertices[i + 3].TransformedVertex);

		m_pMeshDrawComponent->AddQuad(quad);
	}
	m_pMeshDrawComponent->UpdateBuffer();
}

void SoftwareSkinningScene_2::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SoftwareSkinningScene_2::CreateMesh(float length)
{
	XMFLOAT3 pos = XMFLOAT3(length / 2, 0, 0);
	XMFLOAT3 offset = XMFLOAT3(length / 2, 2.5f, 2.5f);
	XMFLOAT4 col = XMFLOAT4(1, 0, 0, 0.5f);
	PushBackBox(m_SkinnedVertices, pos, offset, col, true);

	col = XMFLOAT4(0, 1, 0, 0.5f);
	pos = XMFLOAT3(22.5f, 0, 0);
	PushBackBox(m_SkinnedVertices, pos, offset, col, false);
}
