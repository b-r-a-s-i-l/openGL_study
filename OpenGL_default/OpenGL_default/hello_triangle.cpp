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
"layout (location = 0) in vec3 aPos;\n" // 'in' para os valores que ser�o recebidos; 'layout (location = x)' � o local que vamos armazenar a entrada
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Tudo o que definirmos em 'gl_Position' ser� usado como a sa�da do Vertex Shader.
"}\0";

const char *fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n" // 'out' para valores de cores que ser�o manipulados e enviados para a sa�da do Fragment Shader.
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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
	//OBS: sobreposi��o de vert�ces... para evitar usa-se um EBO.

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
	// Fun��o que destina buffers para o objeto array de v�rtices criado. Primeiro argumento � o n�mero de buffers e o segundo o endere�o do objeto.
	glBindVertexArray(VAO); // Vincula o VAO criado.

	unsigned int EBO; // Element Buffer Object
	glGenBuffers(1, &EBO);
	// Fun��o que destina buffers para o objeto de buffer criado. Primeiro argumento � o n�emro de buffers e o segundo o endere�o do objeto.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Vincula o buffer passado no segundo argumento ao tipo de buffer que � passado no primeiro argumento.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//passa os dados do array 'vertices' ao 'GL_ARRAY_BUFFER' que esta vinculado ao 'VBO'.

	unsigned int VBO; // Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	//---interpreta��o dos atributos de v�rtices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// O primeiro par�metro especifica qual atributo de v�rtice queremos configurar.
	// O pr�ximo argumento especifica o tamanho do atributo de v�rtice.
	// O terceiro argumento especifica o tipo de dado.
	// O pr�ximo argumento especifica se queremos que os dados sejam normalizados. (n�o � relevante, sempre deixar 'GL_FALSE').
	// O quinto argumentoe especifica o espa�o entre atributos de v�rtices consecutivos.
	// O �ltimo par�metro � o deslocamento de onde os dados que queremos come�am no buffer.
	glEnableVertexAttribArray(0); //Ativa o atrributo de v�rtice que foi configurado.

	glBindVertexArray(0); // Quando glBindVertexArray() em '0' desvencula o VAO usado.

	//---SHADER PROGRAM
	unsigned int vertexShader; //Vertex Shader Object
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Tipo de shader que queremos criar como um argumento.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Anexa o c�digo-fonte do shader ao objeto shader
	glCompileShader(vertexShader); //Compila o shader

	//---verificando erros de compila��o
	int  success; // Falso bool para retornar 1 - se n�o falhou e 0 - se falhou.
	char infoLog[512]; //Armazena mensagem de erro.
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Verifica se ouve erros de compila��o no objeto de shader.
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
	shaderProgram = glCreateProgram(); // Cria um programa e retorna a refer�ncia de ID para o objeto de programa rec�m-criado.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);	//glAttachShader() vincula o Shader ao programa de Shader.
	glLinkProgram(shaderProgram); // Linca o programa de shader.

	//---verificando erros de compila��o
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	glUseProgram(shaderProgram); // Usa o programa de shader.

	glDeleteShader(vertexShader); // Para excluir o objeto de shader ap�s 
	glDeleteShader(fragmentShader);

	//---loop de renderiza��o
	while (!glfwWindowShouldClose(window)) // Verifica no in�cio de cada itera��o do loop se o GLFW foi instru�do a fechar.
	{
		//limpa com alguma cor o buufer de tela
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		//---inputs
		processInput(window); // Fun��o para manter todo o c�digo de entrada organizado criado pelo dev.

		//---comandos de renderiza��o
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		/*
		glDrawArrays(GL_TRIANGLES, 0, 3); 
		// Desenha primitivas de VBOs. 
		// Primeira param�tro � o tipo de primitiva, segundo o �ndice inicial da matriz de v�rtices e o terceiro o n�mero de v�rtices.
		*/

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Desenha primitivas de EBOs. 
		// Primeira param�tro � o tipo de primitiva, segundo � o n�mero de elementos a ser desenhado, 
		// O terceiro � o tipo dos �ndices e o �ltimo o deslocamento.
		glBindVertexArray(0);

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

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe Mode
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Normal Mode
	}

}
