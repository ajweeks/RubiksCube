//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MainGame.h"
#include "Base\GeneralStructs.h"
#include "Scenegraph\SceneManager.h"
#include "Physx\PhysxProxy.h"
#include "Diagnostics\DebugRenderer.h"

//#define W1
//#define W2
//#define W3
//#define W4
//#define W5
//#define W6
//#define W7

#define rubikscube
//#define GeometryShader

#ifdef W1
#include "CourseObjects/Week 1/TestScene.h"
#include "CourseObjects/Week 1/ComponentTestScene.h"
#endif // W1

#ifdef W2
#include "CourseObjects/Week 2/ModelTestScene.h"
#include "CourseObjects/Week 2/CharacterTestScene.h"
#endif // W2

#ifdef W3
#include "CourseObjects\Week 3\SpriteTestScene.h"
#include "CourseObjects\Week 3\SpikeyScene.h"
#include "CourseObjects\Week 3\UberMaterialTestScene.h"
#endif // W3

#ifdef W4
#include "CourseObjects\Week 4\PickTestScene.h"
#include "CourseObjects\Week 4\SpriteFontTestScene.h"
#endif // W4

#ifdef W5
#include "CourseObjects\Week 5\CubeMapTestScene.h"
#include "CourseObjects\Week 5\TerrainTestScene.h"
#endif // W5

#ifdef W6
#include "CourseObjects\Week 6\SoftwareSkinningScene_1.h"
#include "CourseObjects\Week 6\SoftwareSkinningScene_2.h"
#include "CourseObjects\Week 6\SoftwareSkinningScene_3.h"
#endif //W6

#ifdef W7
#include "CourseObjects\Week 7\HardwareSkinningScene.h"
#endif

#ifdef rubikscube
#include "CourseObjects\Rubiks Cube\RubiksCubeScene.h"
#endif // rubikscube

#ifdef GeometryShader
#include "CourseObjects\GeometryShader\GeometryShaderTestScene.h"
#endif


MainGame::MainGame()
{
}


MainGame::~MainGame()
{
}

void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
	//Nothing to do atm.
}

void MainGame::Initialize()
{
	SceneManager* sceneManager = SceneManager::GetInstance();

#ifdef W1
	sceneManager->AddGameScene(new TestScene());
	sceneManager->AddGameScene(new ComponentTestScene());

	sceneManager->SetActiveGameScene(L"TestScene");
#endif // W1

#ifdef W2
	sceneManager->AddGameScene(new ModelTestScene());
	sceneManager->AddGameScene(new CharacterTestScene());

	sceneManager->SetActiveGameScene(L"CharacterTestScene");
#endif // W2

#ifdef W3
	sceneManager->AddGameScene(new SpikeyScene());
	sceneManager->AddGameScene(new SpriteTestScene());
	sceneManager->AddGameScene(new UberMaterialTestScene());

	sceneManager->SetActiveGameScene(L"SpriteTestScene");
#endif // W3

#ifdef W4
	sceneManager->AddGameScene(new PickTestScene());
	sceneManager->AddGameScene(new SpriteFontTestScene());

	sceneManager->SetActiveGameScene(L"SpriteFontTestScene");
#endif // W4

#ifdef W5
	sceneManager->AddGameScene(new CubeMapTestScene());
	sceneManager->AddGameScene(new TerrainTestScene());

	sceneManager->SetActiveGameScene(L"TerrainTestScene");
#endif // W5

#ifdef rubikscube
	sceneManager->AddGameScene(new RubiksCubeScene());
	sceneManager->SetActiveGameScene(L"RubiksCubeScene");
#endif // rubikscube

#ifdef W6
	sceneManager->AddGameScene(new SoftwareSkinningScene_1());
	sceneManager->AddGameScene(new SoftwareSkinningScene_2());
	sceneManager->AddGameScene(new SoftwareSkinningScene_3());
	sceneManager->SetActiveGameScene(L"SoftwareSkinningScene_1");
#endif // W6

#ifdef W7
	sceneManager->AddGameScene(new HardwareSkinningScene());
	sceneManager->SetActiveGameScene(L"HardwareSkinningScene");
#endif //W7

#ifdef GeometryShader
	sceneManager->AddGameScene(new GeometryShaderTestScene());
	sceneManager->SetActiveGameScene(L"GeometryShaderTestScene");
#endif // GeometryShader
}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			//NextScene
			if (wParam == VK_F3)
			{
				SceneManager::GetInstance()->NextScene();
				return 0;
			}
			//PreviousScene
			else if (wParam == VK_F2)
			{
				SceneManager::GetInstance()->PreviousScene();
				return 0;
			}
			else if (wParam == VK_F4)
			{
				DebugRenderer::ToggleDebugRenderer();
				return 0;
			}
			else if (wParam == VK_F6)
			{
				auto activeScene = SceneManager::GetInstance()->GetActiveScene();
				activeScene->GetPhysxProxy()->NextPhysXFrame();
			}
		}
	}

	return -1;
}
