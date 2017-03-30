#pragma once
#include "../Base/GeneralStructs.h"

//Forward Declarations
class GameObject;
class SceneManager;
class CameraComponent;
class PhysxProxy;

class GameScene
{
public:
	GameScene(wstring sceneName);
	virtual ~GameScene(void);

	void AddChild(GameObject* obj);
	void RemoveChild(GameObject* obj, bool deleteObject = true);
	const GameContext& GetGameContext() const { return m_GameContext; }

	PhysxProxy * GetPhysxProxy() const { return m_pPhysxProxy; }
	void SetActiveCamera(CameraComponent* pCameraComponent);

protected:

	virtual void Initialize(const GameContext& gameContext) = 0;
	virtual void Update(const GameContext& gameContext) = 0;
	virtual void Draw(const GameContext& gameContext) = 0;
	virtual void SceneActivated(){}
	virtual void SceneDeactivated(){}

private:

	friend class SceneManager;

	void RootInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void RootUpdate();
	void RootDraw();
	void RootSceneActivated();
	void RootSceneDeactivated();
	void RootWindowStateChanged(int state, bool active);

	vector<GameObject*> m_pChildren;
	GameContext m_GameContext;
	bool m_IsInitialized;
	wstring m_SceneName;
	CameraComponent *m_pDefaultCamera, *m_pActiveCamera;
	PhysxProxy* m_pPhysxProxy;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameScene( const GameScene &obj);
	GameScene& operator=( const GameScene& obj);
};
