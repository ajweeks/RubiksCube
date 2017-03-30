//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SceneManager.h"
#include "GameScene.h"
#include "../Base/OverlordGame.h"
#include "../Diagnostics/GameSpecs.h"
#include <algorithm>

SceneManager::SceneManager(void):
	m_pScenes(vector<GameScene*>()),
	m_IsInitialized(false),
	m_ActiveScene(nullptr),
	m_NewActiveScene(nullptr),
	m_pDevice(nullptr),
	m_pDeviceContext(nullptr),
	m_pGame(nullptr)
{
}


SceneManager::~SceneManager(void)
{
	for(GameScene* scene: m_pScenes)
	{
		SafeDelete(scene);
	}
}

void SceneManager::AddGameScene(GameScene* scene)
{
	auto it = find(m_pScenes.begin(), m_pScenes.end(), scene);

	if(it == m_pScenes.end())
	{
		m_pScenes.push_back(scene);

		if(m_IsInitialized)
			scene->RootInitialize(m_pDevice, m_pDeviceContext);

		if(m_ActiveScene==nullptr && m_NewActiveScene==nullptr)
			m_NewActiveScene = scene;
	}
}

void SceneManager::RemoveGameScene(GameScene* scene)
{
	auto it = find(m_pScenes.begin(), m_pScenes.end(), scene);

	if(it != m_pScenes.end())
	{
		m_pScenes.erase(it);
	}
}

void SceneManager::NextScene()
{
	for(UINT i = 0; i < m_pScenes.size(); ++i)
	{
		if(m_pScenes[i] == m_ActiveScene)
		{
			auto nextScene = (++i)%m_pScenes.size();
			m_NewActiveScene = m_pScenes[nextScene];
			break;
		}
	}
}

void SceneManager::PreviousScene()
{
	for(UINT i = 0; i < m_pScenes.size(); ++i)
	{
		if(m_pScenes[i] == m_ActiveScene)
		{
			if(i == 0) i = m_pScenes.size();
			--i;
			m_NewActiveScene = m_pScenes[i];
			break;
		}
	}
}

void SceneManager::SetActiveGameScene(wstring sceneName)
{
	auto it = find_if(m_pScenes.begin(), m_pScenes.end(), [sceneName](GameScene* scene)
	{
		return (wcscmp(scene->m_SceneName.c_str(), sceneName.c_str()) == 0);
	});

	if(it != m_pScenes.end())
	{
		m_NewActiveScene = *it;
	}
}

void SceneManager::WindowStateChanged(int state, bool active) const
{
	if (state == 0 && m_ActiveScene)
	{
		m_ActiveScene->RootWindowStateChanged(state, active);
	}
}

// Update PP
void SceneManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, OverlordGame* game)
{
	if(m_IsInitialized)
		return;

	m_pGame = game;

	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	for(GameScene* scene: m_pScenes)
	{
		scene->RootInitialize(pDevice, pDeviceContext);
	}

	m_IsInitialized = true;
}

void SceneManager::Update()
{
	if(m_NewActiveScene!=nullptr)
	{
		//Deactivate the current active scene
		if(m_ActiveScene!=nullptr)
			m_ActiveScene->RootSceneDeactivated();

		//Set New Scene
		m_ActiveScene = m_NewActiveScene;
		m_NewActiveScene = nullptr;

		//Active the new scene and reset SceneTimer
		m_ActiveScene->RootSceneActivated();
	}

	if(m_ActiveScene!=nullptr)
	{
		GameSpecs::Update(m_ActiveScene->m_GameContext);
		m_ActiveScene->RootUpdate();
	}
}

void SceneManager::Draw()
{
	if(m_ActiveScene!=nullptr)
		m_ActiveScene->RootDraw();
}

OverlordGame* SceneManager::GetGame() const
{
	return m_pGame;
}
