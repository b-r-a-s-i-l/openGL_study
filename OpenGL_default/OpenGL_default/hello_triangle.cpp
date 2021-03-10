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

//---shaders
const char *vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // 'in' para os valores que serão recebidos; 'layout (location = x)' é o local que vamos armazenar a entrada
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Tudo o que definirmos em 'gl_Position' será usado como a saída do Vertex Shader.
"}\0";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n" // 'out' para valores de cores que serão manipulados e enviados para a saída do Fragment Shader.
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

	//---MATRIZ DE MAPEAMENTO
	/*float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};*/
	// hello triangle! :)

	/*float vertices[] = {
		// first triangle
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f,  0.5f, 0.0f,  // top left 
		// second triangle
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};*/
	//OBS: sobreposição de vertíces... para evitar usa-se um EBO.

	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  //0 - top right
	 0.5f, -0.5f, 0.0f,  //1 - bottom right
	-0.5f, -0.5f, 0.0f,  //2 - bottom left
	-0.5f,  0.5f, 0.0f   //3 - top left 
	};
	//hello two triangles! (or rectangle) :)

	unsigned int indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	//---BUFFERS OBJECTS
	unsigned int VAO; // Vertex Array Object
	glGenVertexArrays(1, &VAO); 
	// Função que destina buffers para o objeto array de vértices criado. Primeiro argumento é o número de buffers e o segundo o endereço do objeto.
	glBindVertexArray(VAO); // Vincula o VAO criado.

	unsigned int EBO; // Element Buffer Object
	glGenBuffers(1, &EBO);
	// Função que destina buffers para o objeto de buffer criado. Primeiro argumento é o núemro de buffers e o segundo o endereço do objeto.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Vincula o buffer passado no segundo argumento ao tipo de buffer que é passado no primeiro argumento.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//passa os dados do array 'vertices' ao 'GL_ARRAY_BUFFER' que esta vinculado ao 'VBO'.

	unsigned int VBO; // Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	//---interpretação dos atributos de vértices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// O primeiro parâmetro especifica qual atributo de vértice queremos configurar.
	// O próximo argumento especifica o tamanho do atributo de vértice.
	// O terceiro argumento especifica o tipo de dado.
	// O próximo argumento especifica se queremos que os dados sejam normalizados. (não é relevante, sempre deixar 'GL_FALSE').
	// O quinto argumentoe especifica o espaço entre atributos de vértices consecutivos.
	// O último parâmetro é o deslocamento de onde os dados que queremos começam no buffer.
	glEnableVertexAttribArray(0); //Ativa o atrributo de vértice que foi configurado.

	glBindVertexArray(0); // Quando glBindVertexArray() em '0' desvencula o VAO usado.

	//---SHADER PROGRAM
	unsigned int vertexShader; //Vertex Shader Object
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Tipo de shader que queremos criar como um argumento.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Anexa o código-fonte do shader ao objeto shader
	glCompileShader(vertexShader); //Compila o shader

	//---verificando erros de compilação
	int  success; // Falso bool para retornar 1 - se não falhou e 0 - se falhou.
	char infoLog[512]; //Armazena mensagem de erro.
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Verifica se ouve erros de compilação no objeto de shader.
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Resgata o InfoLog do objeto de shader.
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgram; // Shader Program Object
	shaderProgram = glCreateProgram(); // Cria um programa e retorna a referência de ID para o objeto de programa recém-criado.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);	//glAttachShader() vincula o Shader ao programa de Shader.
	glLinkProgram(shaderProgram); // Linca o programa de shader.

	//---verificando erros de compilação
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	glUseProgram(shaderProgram); // Usa o programa de shader.

	glDeleteShader(vertexShader); // Para excluir o objeto de shader após 
	glDeleteShader(fragmentShader);

	//---loop de renderização
	while (!glfwWindowShouldClose(window)) // Verifica no início de cada iteração do loop se o GLFW foi instruído a fechar.
	{
		//limpa com alguma cor o buufer de tela
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		//---inputs
		processInput(window); // Função para manter todo o código de entrada organizado criado pelo dev.

		//---comandos de renderização
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		/*
		glDrawArrays(GL_TRIANGLES, 0, 3); 
		// Desenha primitivas de VBOs. 
		// Primeira paramêtro é o tipo de primitiva, segundo o índice inicial da matriz de vértices e o terceiro o número de vértices.
		*/

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Desenha primitivas de EBOs. 
		// Primeira paramêtro é o tipo de primitiva, segundo é o número de elementos a ser desenhado, 
		// O terceiro é o tipo dos índices e o último o deslocamento.
		glBindVertexArray(0);

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

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe Mode
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Normal Mode
	}

}
