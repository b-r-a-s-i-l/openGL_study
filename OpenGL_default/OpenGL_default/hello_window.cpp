#include <iostream>
using namespace std;

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//---callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//---main
int main() 
{
	//---inicializa��o GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Vers�o prim�ria definida como 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Vers�o secund�ra definada como 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X

	//---objeto janela
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // Seleciona a window que � passada no argumento

	//---inicializa��o GLAD
	//gladLoadGLLoader() - Fun��o para carregar o endere�o dos ponteiros da fun��o OpenGL que s�o espec�ficos do sistema operacional.
	//glfwGetProcAddress - Define a fun��o correta com base no sistema operacional para o qual estamos compilando.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetFramebufferSizeCallback() - Sempre que a janela muda de tamanho, o GLFW chama a fun��o framebuffer_size_callback() criado pelo dev.

	//---loop de renderiza��o
	while (!glfwWindowShouldClose(window)) // Verifica no in�cio de cada itera��o do loop se o GLFW foi instru�do a fechar.
	{
		//limpa com alguma cor o buufer de tela
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		//---inputs
		processInput(window); // Fun��o para manter todo o c�digo de entrada organizado criado pelo dev.

		//---comandos de renderiza��o

		//---checagem e chamada de eventos de buffer 
		glfwPollEvents();  // Verifica se algum evento � disparado, atualiza o estado da janela e chama as fun��es correspondentes.
		glfwSwapBuffers(window); // Trocar o buffer de cor usado para renderizar durante a renderiza��o e mostr�-lo como sa�da na tela.
	}

	glfwTerminate(); // Limpa todos os recursos e sai do aplicativo corretamente.

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	// Os primeiros dois par�metros define a localiza��o do canto esquerdo inferior da janela 
	// O terceiro e quarto par�metros definem a largura e a altura da janela de renderiza��o em pixels.
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Retorna se esta tecla est� sendo pressionada.
	{
		glfwSetWindowShouldClose(window, true); // Quando true quebra o loop.
	}
}
