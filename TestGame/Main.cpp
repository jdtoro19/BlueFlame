#include <BlueFlameEngine\Engine\BFEngine.h>

using namespace ENGINE;

int main(int argc, char* args[]) {
	
	//Start Engine
	if (BFEngine::GetInstance()->Initialize()) {
		//Game Loop
		BFEngine::GetInstance()->Run();
	}

	return 0;
}