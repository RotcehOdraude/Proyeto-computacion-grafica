#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow *window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX=0.0f;
float movY=0.0f;
float movZ=-5.0f;
float rot = 0.0f;
int main() {
	glfwInit();
	//Verificación de compatibilidad 
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 1", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers
	// use with Orthographic Projection

	//GLfloat vertices[] = {
 //      -0.5f*500, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
	//	0.5f * 500, -0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
	//	0.5f * 500,  0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
	//	0.5f * 500,  0.5f * 500, 0.5f * 500,  1.0f, 0.0f,0.0f,
	//	-0.5f * 500,  0.5f * 500, 0.5f * 500, 1.0f, 0.0f,0.0f,
	//	-0.5f * 500, -0.5f * 500, 0.5f * 500, 1.0f, 0.0f,0.0f,
	//	
	//    -0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,//Back
	//	 0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
	//	 0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
	//	 0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
	//    -0.5f * 500,  0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
	//    -0.5f * 500, -0.5f * 500,-0.5f * 500, 0.0f, 1.0f,0.0f,
	//	
	//	 0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 0.0f,1.0f,
	//	 0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
	//	 0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
	//	 0.5f * 500,  0.5f * 500, -0.5f * 500,  0.0f, 0.0f,1.0f,
	//	 0.5f * 500,  0.5f * 500,  0.5f * 500,  0.0f, 0.0f,1.0f,
	//	 0.5f * 500,  -0.5f * 500, 0.5f * 500, 0.0f, 0.0f,1.0f,
 //     
	//	-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
	//	-0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
	//	-0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
	//	-0.5f * 500, -0.5f * 500, -0.5f * 500,  1.0f, 1.0f,0.0f,
	//	-0.5f * 500, -0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
	//	-0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 1.0f,0.0f,
	//	
	//	-0.5f * 500, -0.5f * 500, -0.5f * 500, 0.0f, 1.0f,1.0f,
	//	0.5f * 500, -0.5f * 500, -0.5f * 500,  0.0f, 1.0f,1.0f,
	//	0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f,1.0f,
	//	0.5f * 500, -0.5f * 500,  0.5f * 500,  0.0f, 1.0f,1.0f,
	//	-0.5f * 500, -0.5f * 500,  0.5f * 500, 0.0f, 1.0f,1.0f,
	//	-0.5f * 500, -0.5f * 500, -0.5f * 500, 0.0f, 1.0f,1.0f,
	//	
	//	-0.5f * 500,  0.5f * 500, -0.5f * 500, 1.0f, 0.2f,0.5f,
	//	0.5f * 500,  0.5f * 500, -0.5f * 500,  1.0f, 0.2f,0.5f,
	//	0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f,0.5f,
	//	0.5f * 500,  0.5f * 500,  0.5f * 500,  1.0f, 0.2f,0.5f,
	//	-0.5f * 500,  0.5f * 500,  0.5f * 500, 1.0f, 0.2f,0.5f,
	//	-0.5f * 500,  0.5f * 500, -0.5f * 500, 1.0f, 0.2f,0.5f,
	//};
	

	// use with Perspective Projection
	float vertices[] = {
		//CUBO 1
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,0.0f,//Front ROJO
		0.5f, -0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,0.0f,
		
	    -0.5f, -0.5f,-0.5f, 0.21f, 0.88f,0.2f,//Back VERDE
		 0.5f, -0.5f,-0.5f, 0.21f, 0.88f,0.2f,
		 0.5f,  0.5f,-0.5f, 0.21f, 0.88f,0.2f,
		 0.5f,  0.5f,-0.5f, 0.21f, 0.88f,0.2f,
	    -0.5f,  0.5f,-0.5f, 0.21f, 0.88f,0.2f,
	    -0.5f, -0.5f,-0.5f, 0.21f, 0.88f,0.2f,
		
		 0.5f, -0.5f,  0.5f,  0.21f, 0.88f,0.2f, // AZUL
		 0.5f, -0.5f, -0.5f,  0.21f, 0.88f,0.2f,
		 0.5f,  0.5f, -0.5f,  0.21f, 0.88f,0.2f,
		 0.5f,  0.5f, -0.5f,  0.21f, 0.88f,0.2f,
		 0.5f,  0.5f,  0.5f,  0.21f, 0.88f,0.2f,
		 0.5f,  -0.5f, 0.5f,  0.21f, 0.88f,0.2f,
      
		-0.5f,  0.5f,  0.5f,  0.21f, 0.88f,0.2f, 
		-0.5f,  0.5f, -0.5f,  0.21f, 0.88f,0.2f,
		-0.5f, -0.5f, -0.5f,  0.21f, 0.88f,0.2f,
		-0.5f, -0.5f, -0.5f,  0.21f, 0.88f,0.2f,
		-0.5f, -0.5f,  0.5f,  0.21f, 0.88f,0.2f,
		-0.5f,  0.5f,  0.5f,  0.21f, 0.88f,0.2f,
		
		-0.5f, -0.5f, -0.5f, 0.21f, 0.88f,0.2f, //AMARILLO
		0.5f, -0.5f, -0.5f,  0.21f, 0.88f,0.2f,
		0.5f, -0.5f,  0.5f,  0.21f, 0.88f,0.2f,
		0.5f, -0.5f,  0.5f,  0.21f, 0.88f,0.2f,
		-0.5f, -0.5f,  0.5f, 0.21f, 0.88f,0.2f,
		-0.5f, -0.5f, -0.5f, 0.21f, 0.88f,0.2f,
		
		-0.5f,  0.5f, -0.5f, 0.21f, 0.88f,0.2f,
		0.5f,  0.5f, -0.5f,  0.21f, 0.88f,0.2f,
		0.5f,  0.5f,  0.5f,  0.21f, 0.88f,0.2f,
		0.5f,  0.5f,  0.5f,  0.21f, 0.88f,0.2f,
		-0.5f,  0.5f,  0.5f, 0.21f, 0.88f,0.2f,
		-0.5f,  0.5f, -0.5f, 0.21f, 0.88f,0.2f,

		//CUBO 2
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,1.0f,//Front ROJO
		0.5f, -0.5f, 0.5f,  1.0f, 1.0f,1.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 1.0f,1.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 1.0f,1.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 1.0f,1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f,1.0f,

		-0.5f, -0.5f,-0.5f, 0.0f, 1.0f,1.0f,//Back VERDE
		 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,1.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,1.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,1.0f,
		-0.5f,  0.5f,-0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,-0.5f, 0.0f, 1.0f,1.0f,

		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f, // AZUL
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		 0.5f,  -0.5f, 0.5f,  0.0f, 1.0f,1.0f,

		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f, //AMARILLO
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,1.0f,

		//CUBO 3
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,0.0f,//Front ROJO
		0.5f, -0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  0.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 0.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,0.0f,

		-0.5f, -0.5f,-0.5f, 0.0f, 0.0f,0.0f,//Back VERDE
		 0.5f, -0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		-0.5f,  0.5f,-0.5f, 0.0f, 0.0f,0.0f,
		-0.5f, -0.5f,-0.5f, 0.0f, 0.0f,0.0f,

		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f, // AZUL
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,0.0f,

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f, //AMARILLO
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,0.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,0.0f,

		};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.3f, -2.50f),		//cabeza
		glm::vec3(0.0f,  0.84f, -1.3f),		//arriba
		glm::vec3(0.0f,  -0.5f, -1.3f),		//abajo
		glm::vec3(4.5f,  0.5f, -1.3f),	    //izq
		glm::vec3(-4.5f,  0.5f, -1.3f),		//der
		glm::vec3(-2.0f, 6.0f, -7.0f),		//cuerno derecha
		glm::vec3(2.0f, 6.0f, -7.0f),		//cuerno izq
		glm::vec3(0.0f, 1.3f, -2.7f),		//ojo 
		glm::vec3(0.0f, 2.3f, -3.2f),		//ojo negro
		glm::vec3(0.0f, 1.0f, -7.0f),		//diente

		glm::vec3(1.3f,  -0.7f, -7.0f),		//pierna izq
		glm::vec3(-1.3f, -0.7f, -7.0f),	    //pierna der
		glm::vec3(0.6f, -4.2f, -2.5f),		//pie izq
		glm::vec3(-0.6f, -4.2f, -2.5f),		//pie der
		glm::vec3(2.3f, -4.2f, -5.0f),		//uña izq 1
		glm::vec3(0.8f, -4.2f, -5.0f),		//uña der 1 
		glm::vec3(-0.8f, -4.2f, -5.0f),		//uña izq 2
		glm::vec3(-2.3f, -4.2f, -5.0f),		//uña der 2

		glm::vec3(3.9f,  1.5f, -6.5f),		//hombro izq
		glm::vec3(-3.9f, 1.5f, -6.5f),	    //hombro der
		glm::vec3(4.9f,  0.1f, -6.5f),		//brazo izq
		glm::vec3(-4.9f, 0.1f, -6.5f),		//brazo der
		glm::vec3(4.9f, -1.0f, -3.2f),		//mano izq
		glm::vec3(4.9f, -2.0f, -8.0f),		// 
		glm::vec3(-4.9f, -1.0f, -3.2f),		//mano der
		glm::vec3(-4.9f, -2.0f, -8.0f)		//
	};


	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	
	glm::mat4 projection=glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		
		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model=glm::mat4(1);
		glm::mat4 view=glm::mat4(1);
	

		//model = glm::rotate(model, (GLfloat)glfwGetTime() * 1.0f, glm::vec3(1.0f,1.0f,-1.0f));
		view = glm::translate(view, glm::vec3(movX,movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate( model, 0.5f, glm::vec3( 1.0f, 0.0f, 0.0f ) ); // use to compare orthographic and perspective projection
		//view = glm::translate( view, glm::vec3( screenWidth / 2, screenHeight / 2,-700.0f ) ); // use with orthographic projection

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	

		glBindVertexArray(VAO);

		//CABEZA fondo
	    model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));
	    model = glm::translate(model, cubePositions[0]);
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////CABEZA arriba
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 1.2f, 1.5f));
		model = glm::translate(model, cubePositions[1]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//CABEZA abajo
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f, 0.4f, 1.5f));
		model = glm::translate(model, cubePositions[2]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////CABEZA izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 1.5f));
		model = glm::translate(model, cubePositions[3]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//CABEZA der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.5f, 1.5f));
		model = glm::translate(model, cubePositions[4]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//cuerno derecha
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, cubePositions[5]);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);
		
		//cuerno IZQUIERDA
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, cubePositions[6]);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);

		//OJO
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.0f, 0.8f, 0.5f));
		model = glm::translate(model, cubePositions[7]);
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);

		//ojo negro
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::translate(model, cubePositions[8]);
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 72, 36);

		//dientes
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(1.2f, 0.3f, 0.2f));
		model = glm::translate(model, cubePositions[9]);
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);

		//Pierna izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 1.2f, 0.3f));
		model = glm::translate(model, cubePositions[10]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pierna der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 1.2f, 0.3f));
		model = glm::translate(model, cubePositions[11]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pie izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.8f, 0.3f, 0.8f));
		model = glm::translate(model, cubePositions[12]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//pie der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.8f, 0.3f, 0.8f));
		model = glm::translate(model, cubePositions[13]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//uña izq 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, cubePositions[14]);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);

		//uña der 1
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, cubePositions[15]);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);

		//uña izq 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, cubePositions[16]);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);

		//uña der 2
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, cubePositions[17]);
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 36, 36);

		//hombro izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, cubePositions[18]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//hombro der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::translate(model, cubePositions[19]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//brazo izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.3f));
		model = glm::translate(model, cubePositions[20]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//brazo der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 1.0f, 0.3f));
		model = glm::translate(model, cubePositions[21]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//brazo izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.6f));
		model = glm::translate(model, cubePositions[22]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo izq
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		model = glm::translate(model, cubePositions[23]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//brazo der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.6f, 0.6f));
		model = glm::translate(model, cubePositions[24]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//dedo der
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.2f));
		model = glm::translate(model, cubePositions[25]);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);


	
		// Swap the screen buffers
		glfwSwapBuffers(window);
	
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
 }

 void Inputs(GLFWwindow *window) {
	 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		 glfwSetWindowShouldClose(window, true);
	 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		 movX += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		 movX -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		 movY += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		 movY -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		 movZ -= 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		 movZ += 0.08f;
	 if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		 rot += 0.4f;
	 if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		 rot -= 0.4f;
 }


