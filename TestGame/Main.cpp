#include <BlueFlameEngine\Engine\BFEngine.h>
#include "Scenes\TestScene.h"

using namespace ENGINE;
using namespace GAME;

int main(int argc, char* args[]) {	

	//Start Engine
	if (BFEngine::GetInstance()->Initialize()) {	
		BFEngine::GetInstance()->GetSceneManager()->SwitchScene(new TestScene());
		//Game Loop
		BFEngine::GetInstance()->Run();
	}

	return 0;
}