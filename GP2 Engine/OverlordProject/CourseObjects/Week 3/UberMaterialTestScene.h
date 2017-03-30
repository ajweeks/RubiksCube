#pragma once

#include "Scenegraph/GameScene.h"

class UberMaterialTestScene : public GameScene
{
public:
	UberMaterialTestScene(void);
	virtual ~UberMaterialTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	GameObject* m_pTeapot;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	UberMaterialTestScene(const UberMaterialTestScene &obj);
	UberMaterialTestScene& operator=(const UberMaterialTestScene& obj);
};

