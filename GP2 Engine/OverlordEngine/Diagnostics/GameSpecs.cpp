//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GameSpecs.h"
#include "../Base/GameTime.h"
#include "Logger.h"
#include "../Base/GeneralStructs.h"

int GameSpecs::m_FPS = 0;
float GameSpecs::m_FpsElapsed = 0.0f;
int GameSpecs::m_FpsFrames = 0;

GameSpecs::GameSpecs(void)
{
}


GameSpecs::~GameSpecs(void)
{
}

void GameSpecs::Update(const GameContext& context)
{
	++m_FpsFrames;
	m_FpsElapsed += context.pGameTime->GetElapsed();
	if(m_FpsElapsed >= 1.0f)
	{
		m_FPS = m_FpsFrames;
		m_FpsFrames = 0;
		m_FpsElapsed -= 1.0f;

		//wstringstream ss;
		//ss<<"FPS: ";
		//ss<<m_FPS;

		//Logger::LogInfo(ss.str());
	}
}
