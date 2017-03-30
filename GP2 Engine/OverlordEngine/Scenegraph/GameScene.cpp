//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GameScene.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "../Diagnostics/Logger.h"
#include "../Prefabs/Prefabs.h"
#include "../Components/Components.h"
#include "../Diagnostics/DebugRenderer.h"
#include "../Physx/PhysxProxy.h"
#include "../Graphics/TextRenderer.h"
#include <SoundManager.h>

GameScene::GameScene(wstring sceneName):
	m_pChildren(vector<GameObject*>()),
	m_GameContext(GameContext()),
	m_IsInitialized(false),
	m_SceneName(sceneName),
	m_pDefaultCamera(nullptr),
	m_pActiveCamera(nullptr),
	m_pPhysxProxy(nullptr)
{
}

GameScene::~GameScene(void)
{
	SafeDelete(m_GameContext.pGameTime);
	SafeDelete(m_GameContext.pInput);
	SafeDelete(m_GameContext.pMaterialManager);

	for(GameObject* pChild:m_pChildren)
	{
		SafeDelete(pChild);
	}
	SafeDelete(m_pPhysxProxy);
}

void GameScene::AddChild(GameObject* obj)
{
#if _DEBUG
	if(obj->m_pParentScene)
	{
		if(obj->m_pParentScene == this)
			Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to this GameScene");
		else
			Logger::LogWarning(L"GameScene::AddChild > GameObject is already attached to another GameScene. Detach it from it's current scene before attaching it to another one.");

		return;
	}

	if(obj->m_pParentObject)
	{
		Logger::LogWarning(L"GameScene::AddChild > GameObject is currently attached to a GameObject. Detach it from it's current parent before attaching it to another one.");
		return;
	}
#endif

	obj->m_pParentScene = this;
	obj->RootInitialize(m_GameContext);
	m_pChildren.push_back(obj);
}

void GameScene::RemoveChild(GameObject* obj, bool deleteObject)
{
	auto it = find(m_pChildren.begin(), m_pChildren.end(), obj);

#if _DEBUG
	if(it == m_pChildren.end())
	{
		Logger::LogWarning(L"GameScene::RemoveChild > GameObject to remove is not attached to this GameScene!");
		return;
	}
#endif

	m_pChildren.erase(it);
	if(deleteObject)
	{
		delete obj;
		obj = nullptr;
	}
	else
		obj->m_pParentScene = nullptr;
}

void GameScene::RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	if(m_IsInitialized)
		return;

	//Create DefaultCamera
	auto freeCam = new FreeCamera();
	freeCam->SetRotation(30, 0);
	freeCam->GetTransform()->Translate(0,50,-80);
	AddChild(freeCam);
	m_pDefaultCamera = freeCam->GetComponent<CameraComponent>();
	m_pActiveCamera = m_pDefaultCamera;
	m_GameContext.pCamera = m_pDefaultCamera;

	//Create GameContext
	m_GameContext.pGameTime = new GameTime();
	m_GameContext.pGameTime->Reset();
	m_GameContext.pGameTime->Stop();

	m_GameContext.pInput = new InputManager();
	m_GameContext.pInput->Initialize();

	m_GameContext.pMaterialManager = new MaterialManager();

	m_GameContext.pDevice = pDevice;
	m_GameContext.pDeviceContext = pDeviceContext;

	// Initialize Physx
	m_pPhysxProxy = new PhysxProxy();
	m_pPhysxProxy->Initialize(this);

	//User-Scene Initialize
	Initialize(m_GameContext);

	//Root-Scene Initialize
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootInitialize(m_GameContext);
	}

	m_IsInitialized = true;
}

void GameScene::RootUpdate()
{
	m_GameContext.pGameTime->Update();
	m_GameContext.pInput->Update();
	m_GameContext.pCamera = m_pActiveCamera;

	SoundManager::GetInstance()->GetSystem()->update();

	//User-Scene Update
	Update(m_GameContext);

	//Root-Scene Update
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootUpdate(m_GameContext);
	}

	m_pPhysxProxy->Update(m_GameContext);
}

void GameScene::RootDraw()
{
	//User-Scene Draw
	Draw(m_GameContext);

	//Object-Scene Draw
	for(GameObject* pChild:m_pChildren)
	{
		pChild->RootDraw(m_GameContext);
	}

	//Object-Scene Post-Draw
	for (GameObject* pChild : m_pChildren)
	{
		pChild->RootPostDraw(m_GameContext);
	}

	//Draw PhysX
	m_pPhysxProxy->Draw(m_GameContext);

	TextRenderer::GetInstance()->Draw(m_GameContext);

	//Draw Debug Stuff
	DebugRenderer::Draw(m_GameContext);
}

void GameScene::RootSceneActivated()
{
	//Start Timer
	m_GameContext.pGameTime->Start();
	SceneActivated();
}

void GameScene::RootSceneDeactivated()
{
	//Stop Timer
	m_GameContext.pGameTime->Stop();
	SceneDeactivated();
}

void GameScene::RootWindowStateChanged(int state, bool active)
{
	//TIMER
	if (state == 0)
	{
		if (active)m_GameContext.pGameTime->Start();
		else m_GameContext.pGameTime->Stop();
	}
}

void GameScene::SetActiveCamera(CameraComponent* pCameraComponent)
{ 
	//nullptr for DefaultCamera

	if(m_pActiveCamera!=nullptr)
		m_pActiveCamera->m_IsActive = false;

	m_pActiveCamera = (pCameraComponent)?pCameraComponent:m_pDefaultCamera;
	m_pActiveCamera->m_IsActive = true;
}