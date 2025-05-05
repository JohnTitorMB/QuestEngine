#include "QuestEngine.h"
/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>*/

#include <crtdbg.h> 
#include <cstdlib>  
#include "CustomColorRGB.h"
#include "ColorManagement/ColorConversion.h"
#include "ColorManagement/RGBModel.h"
/*
int main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(2140);
	

	QuestEngine questEngine = QuestEngine();
	questEngine.Init();
	questEngine.Update();

	return 0;
}

*/

// Callback pour ajuster la fen�tre lors du redimensionnement
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Fonction pour g�rer les inputs (esc pour fermer la fen�tre)
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



int main() {


    QuestEngine questEngine = QuestEngine();
    questEngine.Init();
    questEngine.Update();
 
    return 0;
}
