#include <iostream>
using namespace std;

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>


int main() 
{
	glfwInit(); // inicializa GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Versão primaira definida como 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); // Versão secundára definada como 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X

	//code...

	glfwTerminate();

	return 0;
}