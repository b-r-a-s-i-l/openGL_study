#define STB_IMAGE_IMPLEMENTATION
using namespace std;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Shader.h"

//---settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//---callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() 
{
	//---GLFW load
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X

	//---window object
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//---GLAD load
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//---SHADER
	Shader shader("shaders/ShaderSource.vs", "shaders/ShaderSource.fs");

	//---MATRIZES DE ATRIBUTOS
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   //0 top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   //1 bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   //2 bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    //3 top left 
	};

	unsigned int indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	//---BUFFERS OBJECTS
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//---TEXTURES
	stbi_set_flip_vertically_on_load(true); // inverte o eixo y das imagens para que elas não fiquem de ponta cabeça

	unsigned int texture1; // Texture Object
	glGenTextures(1, &texture1); //primeiro argumento é o número de texturas que queremos gerar, o segundo o endereço da variável.
	glBindTexture(GL_TEXTURE_2D, texture1); // Liga a textura

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //Configura o comportamento do eixo S como 'GL_REPEAT'.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //Configura o comportamento do eixo T como 'GL_REPEAT'.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Configura o filtro minimizado como 'GL_LINEAR'.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Configura o filtro maximizado como 'GL_LINEAR'.

	int width, height, nrChannels;
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
	// stbi_load() - primeiro argumento é a localização do arquivo, seundo, terceiro e quarto serão preenchidos com os valores de largura, altura e número de canais da imagem. O último é irrelevante e sempre passamos '0'.
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //Gera a textura
		// O primeiro argumento especifica o destino ou tipo da textura.
		// O segundo argumento especifica o nível de mipmap para o qual queremos criar uma textura. '0' é o standard.
		// O terceiro argumento informa ao OpenGL em que tipo de formato queremos armazenar a textura. (Conforme número de canais)
		// O 4º e 5º argumento definem a largura e a altura da textura resultante.
		// O próximo argumento deve ser sempre 0.
		// O 7º e 8º argumentos especificam o formato e o tipo de dados da imagem de origem.
		// O último argumento são os dados reais da imagem.
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data); // Libera a memória de imagem.

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);

	//setando texturas ao sampler2D do shader
	shader.use();
	glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); // via manual.
	shader.setInt("texture2", 1); // via método da classe Shader.

	//---LOOP RENDER
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		//---inputs
		processInput(window);

		//---rendering
		shader.use();

		//usando uniform "ourColor " no "fragmentShaderSource"
		float timeValue = glfwGetTime(); // passa o tempo de execução em segundos.
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor"); //localiza o variável uniform através do nome dentro do shader.
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); //setando valores a variável uniform do shader.

		glActiveTexture(GL_TEXTURE0); // Ativa o "slot" de textura.
		glBindTexture(GL_TEXTURE_2D, texture1); // Vincula a textura ao "slot".
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//---checking events
		glfwPollEvents();  // Verifica se algum evento é disparado, atualiza o estado da janela e chama as funções correspondentes.
		glfwSwapBuffers(window); // Trocar o buffer de cor usado para renderizar durante a renderização e mostrá-lo como saída na tela.
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
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
