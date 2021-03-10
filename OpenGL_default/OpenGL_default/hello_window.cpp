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
	//---inicialização GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Versão primária definida como 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Versão secundára definada como 3
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
	glfwMakeContextCurrent(window); // Seleciona a window que é passada no argumento

	//---inicialização GLAD
	//gladLoadGLLoader() - Função para carregar o endereço dos ponteiros da função OpenGL que são específicos do sistema operacional.
	//glfwGetProcAddress - Define a função correta com base no sistema operacional para o qual estamos compilando.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glfwSetFramebufferSizeCallback() - Sempre que a janela muda de tamanho, o GLFW chama a função framebuffer_size_callback() criado pelo dev.

	//---loop de renderização
	while (!glfwWindowShouldClose(window)) // Verifica no início de cada iteração do loop se o GLFW foi instruído a fechar.
	{
		//limpa com alguma cor o buufer de tela
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		//---inputs
		processInput(window); // Função para manter todo o código de entrada organizado criado pelo dev.

		//---comandos de renderização

		//---checagem e chamada de eventos de buffer 
		glfwPollEvents();  // Verifica se algum evento é disparado, atualiza o estado da janela e chama as funções correspondentes.
		glfwSwapBuffers(window); // Trocar o buffer de cor usado para renderizar durante a renderização e mostrá-lo como saída na tela.
	}

	glfwTerminate(); // Limpa todos os recursos e sai do aplicativo corretamente.

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	// Os primeiros dois parâmetros define a localização do canto esquerdo inferior da janela 
	// O terceiro e quarto parâmetros definem a largura e a altura da janela de renderização em pixels.
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Retorna se esta tecla está sendo pressionada.
	{
		glfwSetWindowShouldClose(window, true); // Quando true quebra o loop.
	}
}
