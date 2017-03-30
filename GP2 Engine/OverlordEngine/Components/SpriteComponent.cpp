//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SpriteComponent.h"
#include "../Diagnostics/Logger.h"
#include "../Scenegraph/GameObject.h"
#include "../Graphics/TextureData.h"
#include "../Content/ContentManager.h"
#include "../Graphics/SpriteRenderer.h"
#include "../Components/TransformComponent.h"
#include "../Helpers/MathHelper.h"

SpriteComponent::SpriteComponent(const wstring& spriteAsset, XMFLOAT2 pivot, XMFLOAT4 color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color),
	m_pTexture(nullptr)
{
}

SpriteComponent::~SpriteComponent(void)
{
}

void SpriteComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

void SpriteComponent::Draw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	if (!m_pTexture)
		return;

	TransformComponent* transform = m_pGameObject->GetTransform();
	
	XMFLOAT3 worldPos = transform->GetWorldPosition();
	XMFLOAT3 localPos = transform->GetPosition();
	float depth = localPos.z;
	XMFLOAT4 rotQuat = transform->GetRotation();
	XMFLOAT3 rotEuler = QuaternionToEuler(rotQuat);
	XMFLOAT3 scale = transform->GetScale();
	SpriteRenderer::GetInstance()->Draw(m_pTexture, XMFLOAT2(localPos.x, localPos.y), m_Color, m_Pivot,
		XMFLOAT2(scale.x, scale.y), rotEuler.z, depth);
}
