#include "Engine.h"

int main()
{
	Engine engine = Engine();
	if (engine.init() == GL_FALSE)
	{
		return -1;
	}
	

	if (engine.bufferModels() == GL_FALSE)
	{
		return -1;
	}

	bool suc = engine.useShaders();
	if (suc)
	{
		engine.gameLoop();
	}
	//cin.get();
	//cin.ignore();
	return 0;
}


