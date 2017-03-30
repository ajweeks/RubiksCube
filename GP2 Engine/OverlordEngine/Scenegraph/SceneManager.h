#pragma once
#include "../Helpers/Singleton.h"

//Forward Declarations
class GameScene;
class OverlordGame;

class SceneManager final: public Singleton<SceneManager>
{
public:
	~SceneManager(void);

	void AddGameScene(GameScene* scene);
	void RemoveGameScene(GameScene* scene);
	void SetActiveGameScene(wstring sceneName);
	void NextScene();
	void PreviousScene();
	GameScene* GetActiveScene() const { return m_ActiveScene; }
	OverlordGame* GetGame() const;

private:

	friend class OverlordGame;
	friend class Singleton<SceneManager>;

	SceneManager(void);	

	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, OverlordGame* game);
	void WindowStateChanged(int state, bool active) const;
	void Update();
	void Draw();

	vector<GameScene*> m_pScenes;
	bool m_IsInitialized;
	GameScene* m_ActiveScene, *m_NewActiveScene;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	OverlordGame* m_pGame;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SceneManager( const SceneManager &obj);
	SceneManager& operator=( const SceneManager& obj);
};

