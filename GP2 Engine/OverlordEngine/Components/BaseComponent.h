#pragma once

class GameObject;
class TransformComponent;
struct GameContext;

class BaseComponent
{
public:
	BaseComponent(void);
	virtual ~BaseComponent(void);

	GameObject* GetGameObject() const { return m_pGameObject; }
	TransformComponent* GetTransform() const;

protected:

	virtual void Initialize(const GameContext& gameContext) = 0;
	virtual void Update(const GameContext& gameContext) = 0;
	virtual void Draw(const GameContext& gameContext) = 0;
	virtual void PostDraw(const GameContext& gameContext);

	GameObject* m_pGameObject;
	bool m_IsInitialized;

private:

	friend class GameObject;

	void RootInitialize(const GameContext& gameContext);

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BaseComponent( const BaseComponent &obj);
	BaseComponent& operator=( const BaseComponent& obj);
};


