			/*---------------------------------------------------------*/
			/* ----------------   Projecto Final  ---------------------*/
			/*-----------------       2020-2      ---------------------*/
			/*-------  Alumno: Cabello Aguilar Héctor Eduardo ---------*/

#include <glew.h>
#include <glfw3.h>
#include <stb_image.h>

#include "camera.h"
#include "Model.h"
#include "Texture.h"

// Other Libs
#include "SOIL2/SOIL2.h"

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint skyboxVBO, skyboxVAO;

//Camera
Camera camera(glm::vec3(0.0f, 5.0f, 20.0f));// $$$$$ Posición de la cámara
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, 3.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

void myData(void);
void myData2(void);
void display(Shader, Shader, Model, Model);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f;

//Texture
unsigned int	t_smile;

//Keyframes ?---------------------------- HOMERO
float	posXhomero = 0.0f,
posYhomero = 0.0f,
posZhomero = 0.0f,
giroHomero = 0.0f, // Este es el torso de Homero
rotPiernaDerHomero = 0.0f,
rotPiernaIzqHomero = 0.0f,
movBrazoDerHomero = 0.0f,
movBrazoIzqHomero = 0.0f;
	
// $$$$$$$$ Variables de incremento o diferencia (Delta) que sirve para la interpolacion : (pos siguiente - pos actual) / numero de cuadros
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
giroMonitoInc = 0.0f,
rotIncDer = 0.0f,
rotIncIzq = 0.0f,
movBrazoIncDer = 0.0f,
movBrazoIncIzq = 0.0f;

#define MAX_FRAMES 9    //$$$$$$$$$$$$$$$$$$$$$$$$$$ Es la condicion maxima del ciclo for de los frames
int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float movBrazoDer;
	float movBrazoIzq;
	float rotRodDer;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];   //$$$$$$$$$$$$$$$$$$$$$$$$$$ Frame es una estructura que contine las posiciones de los keyframes
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) //$$$$$$$$$$$$$$$$$$$$$$ Almacena en el arreglo de frames la posicion de todas las variables de todas las partes que se mueven de los modelos 
{

	printf("---- frameindex %d ----\n", FrameIndex);

	KeyFrame[FrameIndex].posX = posXhomero;				printf("KeyFrame[%i].posX = %f\n",FrameIndex, posXhomero);
	KeyFrame[FrameIndex].posY = posYhomero;				printf("KeyFrame[%i].posY = %f\n", FrameIndex, posYhomero);
	KeyFrame[FrameIndex].posZ = posZhomero;				printf("KeyFrame[%i].posZ = %f\n", FrameIndex, posZhomero);

	KeyFrame[FrameIndex].giroMonito = giroHomero;			printf("KeyFrame[%i].giroMonito = %f\n", FrameIndex, giroHomero);
	KeyFrame[FrameIndex].rotRodDer = rotPiernaDerHomero;			printf("KeyFrame[%i].rotRodDer = %f\n", FrameIndex, rotPiernaDerHomero);
	KeyFrame[FrameIndex].rotRodIzq = rotPiernaIzqHomero;			printf("KeyFrame[%i].rotRodIzq = %f\n", FrameIndex, rotPiernaIzqHomero);
	KeyFrame[FrameIndex].movBrazoDer = movBrazoDerHomero;			printf("KeyFrame[%i].movBrazoDer = %f\n", FrameIndex, movBrazoDerHomero);
	KeyFrame[FrameIndex].movBrazoIzq = movBrazoIzqHomero;			printf("KeyFrame[%i].movBrazoIzq = %f\n", FrameIndex, movBrazoIzqHomero);

	FrameIndex++;
}

void resetElements(void) //$$$$$$$$$$ Este metodo se manda a llamar justo en el instante que se decide reproducir el animacion, establece los valores de los modelos a los valore del primer frame
{
	posXhomero = KeyFrame[0].posX;
	posYhomero = KeyFrame[0].posY;
	posZhomero = KeyFrame[0].posZ;

	giroHomero = KeyFrame[0].giroMonito;

	rotPiernaDerHomero = KeyFrame[0].rotRodDer;
	rotPiernaIzqHomero = KeyFrame[0].rotRodIzq;

	movBrazoDerHomero = KeyFrame[0].movBrazoDer;
	movBrazoIzqHomero = KeyFrame[0].movBrazoIzq;
}

void interpolation(void)
{   //$$$$$$$ Las variables de incremeto calculan el cambio que hubo en todas las variables para despues hacer la interpolacion

	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;
	
	rotIncDer = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	rotIncIzq = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;

	movBrazoIncDer = (KeyFrame[playIndex + 1].movBrazoDer - KeyFrame[playIndex].movBrazoDer) / i_max_steps;
	movBrazoIncIzq = (KeyFrame[playIndex + 1].movBrazoIzq - KeyFrame[playIndex].movBrazoIzq) / i_max_steps;

}


unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

	//$$$$$$$$$$$$$$$$$$$$$$$$$$ Deberia estar vacio?
}

void myData()
{	
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void myData2()
{
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	// SkyBox
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

}


void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations   //$$$$$$$$$$$ Esta condicion calcula la interpolacion de cada frame en el arreglo 
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation   //$$$$$$$$$$$ Los incrementos tienen el calculo de la interpolacion...
			posXhomero += incX; 
			posYhomero += incY;
			posZhomero += incZ;

			giroHomero += giroMonitoInc;
			rotPiernaDerHomero += rotIncDer;
			rotPiernaIzqHomero += rotIncIzq;
			movBrazoDerHomero += movBrazoIncDer;
			movBrazoIzqHomero += movBrazoIncIzq;

			i_curr_steps++;
		}

	}
	
}

void display(	Shader shader, Shader skyboxShader, GLuint skybox, 
				Model torso, Model piernaDer, Model piernaIzq,
				Model brazoDer, Model brazoIzq, Model cabeza, Model escenario) //$$$$$ Funcion display preconfigurada
{
	shader.use();

	//Setup Advanced Lights
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("dirLight.direction", lightDirection);
	shader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	shader.setVec3("pointLight[0].position", lightPosition);
	shader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[0].constant", 1.0f);
	shader.setFloat("pointLight[0].linear", 0.22f);
	shader.setFloat("pointLight[0].quadratic", 0.019f);

	shader.setVec3("pointLight[1].position", glm::vec3(20.0, 15.0f, 35.0f));
	shader.setVec3("pointLight[1].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	shader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
	shader.setFloat("pointLight[1].constant", 1.0f);
	shader.setFloat("pointLight[1].linear", 0.022f);
	shader.setFloat("pointLight[1].quadratic", 0.0019f);

	shader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 tmp = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera //$$$$$$$$$ Profundidad del plano de la camara ---> ultimo valor
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	//Escenario
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -100.0f));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial del escenario
	//model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));// <--- tamaño del escenario .. NO ESALAR
	shader.setMat4("model", model);
	escenario.Draw(shader);

	//Homero torso
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de homero
	model = glm::translate(model, glm::vec3(posXhomero, posYhomero, posZhomero));//$$$$$$$$$$$$$$$$$$$$$$$$$$ posicion del modelo en funcion del teclado
	tmp = model = glm::rotate(model, glm::radians(giroHomero), glm::vec3(0.0f, 1.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
	shader.setMat4("model", model);
	torso.Draw(shader);

	//Pierna Der Homero 
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(rotPiernaDerHomero), glm::vec3(-1.0f, 0.0f, 0.0f)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro de rotRodIzq
	shader.setMat4("model", model);
	piernaDer.Draw(shader);

	/*Pie derecho antiguo
	model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
	shader.setMat4("model", model);
	botaDer.Draw(shader);*/

	//Pierna Izq Homero
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -0.0f));
	model = glm::rotate(model, glm::radians(rotPiernaIzqHomero), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	piernaIzq.Draw(shader);

	/*Pie Izq antiguo
	model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
	shader.setMat4("model", model);
	botaDer.Draw(shader);	*/

	//Brazo derecho Homero
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(movBrazoDerHomero), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	brazoDer.Draw(shader);

	//Brazo izquierdo Homero
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(movBrazoIzqHomero), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	brazoIzq.Draw(shader);

	//Cabeza Homero
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
	shader.setMat4("model", model);
	cabeza.Draw(shader);


	// Draw skybox as last
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix

	skyboxShader.setMat4("view", view);
	skyboxShader.setMat4("projection", projection);

	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // Set depth function back to default
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Projecto CompuGrafica Hector 2020-2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, my_input);  //$$$$$$$$$$$ Aqui se lee por cada ciclo el estado de las teclas del teclado ?? creo...
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();    //$$$$$$$$$$$$$$$$$$$$$$$$$$
	myData2();
	glEnable(GL_DEPTH_TEST);
	
	//Shaders
	Shader modelShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs"); //$$$$$$$$$$$$$$$$$$$$$$$$$$ checar si este shader influye en el monito ...
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");

	// Las partes de Homero
	Model torso = ((char *)"Models/Personaje/torso.obj");
	Model piernaDer = ((char *)"Models/Personaje/piernaDer.obj");
	Model piernaIzq = ((char *)"Models/Personaje/piernaIzq.obj");
	Model brazoDer = ((char *)"Models/Personaje/brazoDer.obj");
	Model brazoIzq = ((char *)"Models/Personaje/brazoIzq.obj");
	Model cabeza = ((char *)"Models/Personaje/cabeza.obj");
	Model escenario = ((char *)"Models/Escenario/escenario2.obj");

	//Inicialización de todos los KeyFrames a un valor de cero
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	
	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");//$$$$$$$$$$$$$$$$$$$$$$$$$$ Aqui puedo cambiar las caras de mi skybox
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);//$$$ Configura la camara, puede ser perspective o ortho(ortogonal) ademas de la profundidad del plano de la camara
	// render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        //my_input(window);
		animate(); //$$$$$$$$$$$$$$$$$$$$$$$$$$ esta a la espera de que se defina la animacion por keyframes con la espera de
		////$$$$$$$$$$$$$$$$$$$$$$$$$$ la tecla espacio 

        // render
        // Background color
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//display(modelShader, ourModel, llantasModel);
		display(modelShader, SkyBoxshader, cubemapTexture, 
				torso, piernaDer,piernaIzq, brazoDer, brazoIzq, cabeza, escenario); //$$$$$$$$$$$$$$$$$$$$$$$$$$ carga las texturas, los modelos en el display

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//Posicion del modelo
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		posXhomero++;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		posXhomero--;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		posYhomero++;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		posYhomero--;
	
	// Movimiento de sus articulaciones
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		giroHomero++;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		giroHomero--;
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotPiernaDerHomero++;
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		rotPiernaDerHomero--;
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		rotPiernaIzqHomero++;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		rotPiernaIzqHomero--;
	if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
		movBrazoDerHomero++;
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		movBrazoDerHomero--;
	
	//To play KeyFrame animation 
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))  //$$$$$$ Se reproduce el KeyFrame si y solo si se guradó por lo menos un frame y play esta en falso
		{
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{ //$$$$$ En caso de que por alguna razon? el valor de play fura True
			play = false;
		}
	}

	//To Save a KeyFrame
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame(); //$$$$$$$$$$$$$$$$ guarda un frame
		}
		else {
			printf("$$Se acabaron los frames");
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}