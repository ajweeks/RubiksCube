#pragma once
#include "../Helpers/Singleton.h"

class SoundManager: public Singleton<SoundManager>
{
public:
	SoundManager(void);
	~SoundManager(void);

	static bool ErrorCheck(FMOD_RESULT res);
	FMOD::System* GetSystem() const { return m_pFmodSystem; }

private:
	void Initialize();
	FMOD::System* m_pFmodSystem = nullptr;

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SoundManager(const SoundManager &obj) = delete;
	SoundManager& operator=(const SoundManager& obj) = delete;

};

