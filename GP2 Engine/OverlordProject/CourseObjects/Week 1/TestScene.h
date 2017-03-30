#pragma once
#include <Scenegraph/GameScene.h>

class TestScene: public GameScene
{
public:
	TestScene();
	~TestScene();

	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void SceneActivated() override;
	void SceneDeactivated() override;

private:

	GameObject* m_pTorus0 = nullptr;
	GameObject* m_pTorus1 = nullptr;
	GameObject* m_pTorus2 = nullptr;

	float m_TotalRotation = 0.f;

private:
	TestScene(const TestScene &obj) = delete;
	TestScene& operator=(const TestScene& obj) = delete;
};

