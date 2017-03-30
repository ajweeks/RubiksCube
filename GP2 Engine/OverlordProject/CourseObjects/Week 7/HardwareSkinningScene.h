#pragma once
#include "Scenegraph/GameScene.h"

class ModelComponent;
class ModelAnimator;

class HardwareSkinningScene : public GameScene
{
public:
	HardwareSkinningScene();
	virtual ~HardwareSkinningScene();

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	void UpdateAnimator(ModelAnimator* animator, const GameContext& gameContext);

	ModelComponent* m_pKnightModel;
	ModelComponent* m_pBikeModel;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	HardwareSkinningScene(const HardwareSkinningScene &obj);
	HardwareSkinningScene& operator=(const HardwareSkinningScene& obj);
};

