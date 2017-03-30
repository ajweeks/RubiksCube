#pragma once

struct GameContext;

class GameSpecs
{
public:
	GameSpecs(void);
	~GameSpecs(void);

	static void Update(const GameContext& context);
	static int GetFPS() { return m_FPS; }

private:

	static int m_FPS;
	static float m_FpsElapsed;
	static int m_FpsFrames;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	GameSpecs( const GameSpecs &obj);
	GameSpecs& operator=( const GameSpecs& obj);
};

