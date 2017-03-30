#pragma once
#include "Scenegraph/GameScene.h"

class ModelTestScene : public GameScene
{
public:
	ModelTestScene(void);
	virtual ~ModelTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	GameObject* m_pChair;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ModelTestScene(const ModelTestScene &obj);
	ModelTestScene& operator=(const ModelTestScene& obj);
};

