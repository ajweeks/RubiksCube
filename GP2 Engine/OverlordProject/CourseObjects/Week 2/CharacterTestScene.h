#pragma once

#include "Scenegraph/GameScene.h"

class Character;

class CharacterTestScene : public GameScene
{
public:
	CharacterTestScene(void);
	virtual ~CharacterTestScene(void);

protected:

	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:

	Character* m_pCharacter;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	CharacterTestScene(const CharacterTestScene &obj);
	CharacterTestScene& operator=(const CharacterTestScene& obj);
};

