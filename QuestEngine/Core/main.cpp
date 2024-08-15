#include "QuestEngine.h"
/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>*/

#include <crtdbg.h> 
#include <cstdlib>  


int main()
{
/*	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(2140);
	*/

	QuestEngine questEngine = QuestEngine();
	questEngine.Init();
	questEngine.Update();

	return 0;
}

