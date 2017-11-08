#include <BlueFlameEngine\Engine\BFEngine.h>
#include "Scenes\TestScene.h"
#include "Scenes\DemoScene.h"

using namespace ENGINE;
using namespace GAME;

int main(int argc, char* args[]) {	

	//Start Engine
	if (BFEngine::GetInstance()->Initialize()) {	
		BFEngine::GetInstance()->GetSceneManager()->SwitchScene(new DemoScene());
		//Game Loop
		BFEngine::GetInstance()->Run();
	}

	return 0;
}