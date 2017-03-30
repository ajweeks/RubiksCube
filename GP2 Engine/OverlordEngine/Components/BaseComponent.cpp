//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "BaseComponent.h"
#include "../Scenegraph/GameObject.h"
#include "../Diagnostics/Logger.h"

BaseComponent::BaseComponent(void):
	m_IsInitialized(false),
	m_pGameObject(nullptr)
{
}


BaseComponent::~BaseComponent(void)
{
}

void BaseComponent::RootInitialize(const GameContext& gameContext)
{
	if(m_IsInitialized)
		return;

	Initialize(gameContext);

	m_IsInitialized = true;
}

void BaseComponent::PostDraw(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
}

TransformComponent* BaseComponent::GetTransform() const
{
#if _DEBUG
	if(!m_pGameObject)
	{
		Logger::LogWarning(L"BaseComponent::GetTransform() > Failed to retrieve the TransformComponent. GameObject is NULL.");
		return nullptr;
	}
#endif

	return m_pGameObject->GetTransform();
}
