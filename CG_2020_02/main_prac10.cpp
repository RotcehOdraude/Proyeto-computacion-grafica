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
//#define MAX_FRAMES 9;   
const int max_frames = 30;
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
// ------------------------------------- Marge
float	posXmarge = 0.0f,
		posYmarge = 0.0f,
		posZmarge = 0.0f,
		giroMarge = 0.0f, // Este es el torso
		rotPiernaDerMarge = 0.0f,
		rotPiernaIzqMarge = 0.0f,
		movBrazoDerMarge = 0.0f,
		movBrazoIzqMarge = 0.0f;
// ------------------------------------- Bart
float	posXbart = 0.0f,
		posYbart = 0.0f,
		posZbart = 0.0f,
		giroBart = 0.0f, // Este es el torso
		rotPiernaDerBart = 0.0f,
		rotPiernaIzqBart = 0.0f,
		movBrazoDerBart = 0.0f,
		movBrazoIzqBart = 0.0f;
//--------------------------------------- Lambo puerta
float	posXlambo = 0.0f,
		posYlambo= 0.0f,
		posZlambo = 0.0f,
		giroLambo = 0.0f,
		posXpuerta = 0.0f,
		posYpuerta = 0.0f,
		posZpuerta = 0.0f,
		giroPuerta = 0.0f;


	
// $$$$$$$$ Variables de incremento o diferencia (Delta) que sirve para la interpolacion : (pos siguiente - pos actual) / numero de cuadros
// Homero
float	homeroIncX = 0.0f,
		homeroIncY = 0.0f,
		homeroIncZ = 0.0f,
		homeroGiroInc = 0.0f,
		homeroPierDerInc = 0.0f,
		homeroPierIzqInc = 0.0f,
		homeroBrazoDerInc = 0.0f,
		homeroBrazoIzqInc = 0.0f;
//Marge
float	margeIncX = 0.0f,
		margeIncY = 0.0f,
		margeIncZ = 0.0f,
		margeGiroInc = 0.0f,
		margePierDerInc = 0.0f,
		margePierIzqInc = 0.0f,
		margeBrazoDerInc = 0.0f,
		margeBrazoIzqInc = 0.0f;
//Bart
float	bartIncX = 0.0f,
		bartIncY = 0.0f,
		bartIncZ = 0.0f,
		bartGiroInc = 0.0f,
		bartPierDerInc = 0.0f,
		bartPierIzqInc = 0.0f,
		bartBrazoDerInc = 0.0f,
		bartBrazoIzqInc = 0.0f;
//Lambo Puerta
float	lamboIncX = 0.0f,
		lamboIncY = 0.0f,
		lamboIncZ = 0.0f,
		lamboGiroInc = 0.0f,
		puertaIncX = 0.0f,
		puertaIncY = 0.0f,
		puertaIncZ = 0.0f,
		puertaGiroInc = 0.0f;


int i_max_steps = 190;
int i_curr_steps = 0;
typedef struct _frame{
//Homero
	float homeroPosX;		
	float homeroPosY;		
	float homeroPosZ;		
	float homeroGiro;
	float homeroPiernaDer;
	float homeroPiernaIzq;
	float homeroBrazoDer;
	float homeroBrazoIzq;
//Marge
	float margePosX;
	float margePosY;
	float margePosZ;
	float margeGiro;
	float margePiernaDer;
	float margePiernaIzq;
	float margeBrazoDer;
	float margeBrazoIzq;
//Bart
	float bartPosX;
	float bartPosY;
	float bartPosZ;
	float bartGiro;
	float bartPiernaDer;
	float bartPiernaIzq;
	float bartBrazoDer;
	float bartBrazoIzq;
//Lambo
	float lamboPosX;
	float lamboPosY;
	float lamboPosZ;
	float lamboGiro;
//Puerta
	float puertaPosX;
	float puertaPosY;
	float puertaPosZ;
	float puertaGiro;



}FRAME;

FRAME KeyFrame[max_frames];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void) //$$$$$$$$$$$$$$$$$$$$$$ Almacena en el arreglo de frames la posicion de todas las variables de todas las partes que se mueven de los modelos 
{

	printf("---- frameindex %d ----\n", FrameIndex);
	//Homero
	KeyFrame[FrameIndex].homeroPosX = posXhomero;						printf("KeyFrame[%i].homeroPosX = %f\n",FrameIndex, posXhomero);
	KeyFrame[FrameIndex].homeroPosY = posYhomero;						printf("KeyFrame[%i].homeroPosY = %f\n", FrameIndex, posYhomero);
	KeyFrame[FrameIndex].homeroPosZ = posZhomero;						printf("KeyFrame[%i].homeroPosZ = %f\n", FrameIndex, posZhomero);

	KeyFrame[FrameIndex].homeroGiro = giroHomero;						printf("KeyFrame[%i].homeroGiro = %f\n", FrameIndex, giroHomero);
	KeyFrame[FrameIndex].homeroPiernaDer = rotPiernaDerHomero;			printf("KeyFrame[%i].homeroPiernaDer = %f\n", FrameIndex, rotPiernaDerHomero);
	KeyFrame[FrameIndex].homeroPiernaIzq = rotPiernaIzqHomero;			printf("KeyFrame[%i].homeroPiernaIzq = %f\n", FrameIndex, rotPiernaIzqHomero);
	KeyFrame[FrameIndex].homeroBrazoDer = movBrazoDerHomero;			printf("KeyFrame[%i].homeroBrazoDer = %f\n", FrameIndex, movBrazoDerHomero);
	KeyFrame[FrameIndex].homeroBrazoIzq = movBrazoIzqHomero;			printf("KeyFrame[%i].homeroBrazoIzq = %f\n", FrameIndex, movBrazoIzqHomero);
	//Marge
	KeyFrame[FrameIndex].margePosX = posXmarge;							printf("KeyFrame[%i].margePosX = %f\n", FrameIndex, posXmarge);
	KeyFrame[FrameIndex].margePosY = posYmarge;							printf("KeyFrame[%i].margePosY = %f\n", FrameIndex, posYmarge);
	KeyFrame[FrameIndex].margePosZ = posZmarge;							printf("KeyFrame[%i].margePosZ = %f\n", FrameIndex, posZmarge);

	KeyFrame[FrameIndex].margeGiro = giroMarge;							printf("KeyFrame[%i].margeGiro = %f\n", FrameIndex, giroMarge);
	KeyFrame[FrameIndex].margePiernaDer = rotPiernaDerMarge;			printf("KeyFrame[%i].margePiernaDer = %f\n", FrameIndex, rotPiernaDerMarge);
	KeyFrame[FrameIndex].margePiernaIzq = rotPiernaIzqMarge;			printf("KeyFrame[%i].margePiernaIzq = %f\n", FrameIndex, rotPiernaIzqMarge);
	KeyFrame[FrameIndex].margeBrazoDer = movBrazoDerMarge;				printf("KeyFrame[%i].margeBrazoDer = %f\n", FrameIndex, movBrazoDerMarge);
	KeyFrame[FrameIndex].margeBrazoIzq = movBrazoIzqMarge;				printf("KeyFrame[%i].margeBrazoIzq = %f\n", FrameIndex, movBrazoIzqMarge);
	//Bart
	KeyFrame[FrameIndex].bartPosX = posXbart;							printf("KeyFrame[%i].bartPosX = %f\n", FrameIndex, posXbart);
	KeyFrame[FrameIndex].bartPosY = posYbart;							printf("KeyFrame[%i].bartPosY = %f\n", FrameIndex, posYbart);
	KeyFrame[FrameIndex].bartPosZ = posZbart;							printf("KeyFrame[%i].bartPosZ = %f\n", FrameIndex, posZbart);

	KeyFrame[FrameIndex].bartGiro = giroBart;							printf("KeyFrame[%i].bartGiro = %f\n", FrameIndex, giroBart);
	KeyFrame[FrameIndex].bartPiernaDer = rotPiernaDerBart;				printf("KeyFrame[%i].bartPiernaDer = %f\n", FrameIndex, rotPiernaDerBart);
	KeyFrame[FrameIndex].bartPiernaIzq = rotPiernaIzqBart;				printf("KeyFrame[%i].bartPiernaIzq = %f\n", FrameIndex, rotPiernaIzqBart);
	KeyFrame[FrameIndex].bartBrazoDer = movBrazoDerBart;				printf("KeyFrame[%i].bartBrazoDer = %f\n", FrameIndex, movBrazoDerBart);
	KeyFrame[FrameIndex].bartBrazoIzq = movBrazoIzqBart;				printf("KeyFrame[%i].bartBrazoIzq = %f\n", FrameIndex, movBrazoIzqBart);
	//lambo
	KeyFrame[FrameIndex].lamboPosX = posXlambo;							printf("KeyFrame[%i].lamboPosX = %f\n", FrameIndex, posXlambo);
	KeyFrame[FrameIndex].lamboPosY = posYlambo;							printf("KeyFrame[%i].lamboPosY = %f\n", FrameIndex, posYlambo);
	KeyFrame[FrameIndex].lamboPosZ = posZlambo;							printf("KeyFrame[%i].lamboPosZ = %f\n", FrameIndex, posZlambo);
	KeyFrame[FrameIndex].lamboGiro = giroLambo;							printf("KeyFrame[%i].lamboGiro = %f\n", FrameIndex, giroLambo);
	//Puerta
	KeyFrame[FrameIndex].puertaPosX = posXpuerta;						printf("KeyFrame[%i].puertaPosX = %f\n", FrameIndex, posXpuerta);
	KeyFrame[FrameIndex].puertaPosY = posYpuerta;						printf("KeyFrame[%i].puertaPosY = %f\n", FrameIndex, posYpuerta);
	KeyFrame[FrameIndex].puertaPosZ = posZpuerta;						printf("KeyFrame[%i].puertaPosZ = %f\n", FrameIndex, posZpuerta);
	KeyFrame[FrameIndex].puertaGiro = giroPuerta;						printf("KeyFrame[%i].puertaPosZ = %f\n", FrameIndex, giroPuerta);

	FrameIndex++;
}

void resetElements(void) //$$$$$$$$$$ Este metodo se manda a llamar justo en el instante que se decide reproducir el animacion, establece los valores de los modelos a los valore del primer frame
{
	// Homero
	posXhomero = KeyFrame[0].homeroPosX;
	posYhomero = KeyFrame[0].homeroPosY;
	posZhomero = KeyFrame[0].homeroPosZ;

	giroHomero = KeyFrame[0].homeroGiro;
	rotPiernaDerHomero = KeyFrame[0].homeroPiernaDer;
	rotPiernaIzqHomero = KeyFrame[0].homeroPiernaIzq;
	movBrazoDerHomero = KeyFrame[0].homeroBrazoDer;
	movBrazoIzqHomero = KeyFrame[0].homeroBrazoIzq;
	//Marge
	posXmarge = KeyFrame[0].margePosX;
	posYmarge = KeyFrame[0].margePosY;
	posZmarge = KeyFrame[0].margePosZ;

	giroMarge = KeyFrame[0].margeGiro;
	rotPiernaDerMarge= KeyFrame[0].margePiernaDer;
	rotPiernaIzqMarge= KeyFrame[0].margePiernaIzq;
	movBrazoDerMarge= KeyFrame[0].margeBrazoDer;
	movBrazoIzqMarge= KeyFrame[0].margeBrazoIzq;
	//Bart
	posXbart = KeyFrame[0].bartPosX;
	posYbart = KeyFrame[0].bartPosY;
	posZbart = KeyFrame[0].bartPosZ;

	giroBart = KeyFrame[0].bartGiro;
	rotPiernaDerBart = KeyFrame[0].bartPiernaDer;
	rotPiernaIzqBart = KeyFrame[0].bartPiernaIzq;
	movBrazoDerBart = KeyFrame[0].bartBrazoDer;
	movBrazoIzqBart = KeyFrame[0].bartBrazoIzq;
	//Lambo puerta
	posXlambo= KeyFrame[0].lamboPosX;
	posYlambo = KeyFrame[0].lamboPosY;
	posZlambo = KeyFrame[0].lamboPosZ;
	giroLambo = KeyFrame[0].lamboGiro;

	posXpuerta = KeyFrame[0].puertaPosX;
	posYpuerta = KeyFrame[0].puertaPosY;
	posZpuerta = KeyFrame[0].puertaPosZ;
	giroPuerta = KeyFrame[0].puertaGiro;


}

void interpolation(void)
{   //$$$$$$$ Las variables de incremeto calculan el cambio que hubo en todas las variables para despues hacer la interpolacion
	//Homero
	homeroIncX = (KeyFrame[playIndex + 1].homeroPosX - KeyFrame[playIndex].homeroPosX) / i_max_steps;
	homeroIncY = (KeyFrame[playIndex + 1].homeroPosY - KeyFrame[playIndex].homeroPosY) / i_max_steps;
	homeroIncZ = (KeyFrame[playIndex + 1].homeroPosZ - KeyFrame[playIndex].homeroPosZ) / i_max_steps;

	homeroGiroInc = (KeyFrame[playIndex + 1].homeroGiro - KeyFrame[playIndex].homeroGiro) / i_max_steps;
	homeroPierDerInc = (KeyFrame[playIndex + 1].homeroPiernaDer - KeyFrame[playIndex].homeroPiernaDer) / i_max_steps;
	homeroPierIzqInc = (KeyFrame[playIndex + 1].homeroPiernaIzq - KeyFrame[playIndex].homeroPiernaIzq) / i_max_steps;
	homeroBrazoDerInc = (KeyFrame[playIndex + 1].homeroBrazoDer - KeyFrame[playIndex].homeroBrazoDer) / i_max_steps;
	homeroBrazoIzqInc = (KeyFrame[playIndex + 1].homeroBrazoIzq - KeyFrame[playIndex].homeroBrazoIzq) / i_max_steps;
	//Marge
	margeIncX = (KeyFrame[playIndex + 1].margePosX - KeyFrame[playIndex].margePosX) / i_max_steps;
	margeIncY = (KeyFrame[playIndex + 1].margePosY - KeyFrame[playIndex].margePosY) / i_max_steps;
	margeIncZ = (KeyFrame[playIndex + 1].margePosZ - KeyFrame[playIndex].margePosZ) / i_max_steps;
	
	margeGiroInc = (KeyFrame[playIndex + 1].margeGiro - KeyFrame[playIndex].margeGiro) / i_max_steps;
	margePierDerInc = (KeyFrame[playIndex + 1].margePiernaDer - KeyFrame[playIndex].margePiernaDer) / i_max_steps;
	margePierIzqInc = (KeyFrame[playIndex + 1].margePiernaIzq - KeyFrame[playIndex].margePiernaIzq) / i_max_steps;
	margeBrazoDerInc = (KeyFrame[playIndex + 1].margeBrazoDer - KeyFrame[playIndex].margeBrazoDer) / i_max_steps;
	margeBrazoIzqInc = (KeyFrame[playIndex + 1].margeBrazoIzq - KeyFrame[playIndex].margeBrazoIzq) / i_max_steps;
	//Bart
	bartIncX = (KeyFrame[playIndex + 1].bartPosX - KeyFrame[playIndex].bartPosX) / i_max_steps;
	bartIncY = (KeyFrame[playIndex + 1].bartPosY - KeyFrame[playIndex].bartPosY) / i_max_steps;
	bartIncZ = (KeyFrame[playIndex + 1].bartPosZ - KeyFrame[playIndex].bartPosZ) / i_max_steps;
	
	bartGiroInc = (KeyFrame[playIndex + 1].bartGiro - KeyFrame[playIndex].bartGiro) / i_max_steps;
	bartPierDerInc = (KeyFrame[playIndex + 1].bartPiernaDer - KeyFrame[playIndex].bartPiernaDer) / i_max_steps;
	bartPierIzqInc = (KeyFrame[playIndex + 1].bartPiernaIzq - KeyFrame[playIndex].bartPiernaIzq) / i_max_steps;
	bartBrazoDerInc = (KeyFrame[playIndex + 1].bartBrazoDer - KeyFrame[playIndex].bartBrazoDer) / i_max_steps;
	bartBrazoIzqInc = (KeyFrame[playIndex + 1].bartBrazoIzq - KeyFrame[playIndex].bartBrazoIzq) / i_max_steps;
	//Lambo puerta
	lamboIncX = (KeyFrame[playIndex + 1].lamboPosX - KeyFrame[playIndex].lamboPosX) / i_max_steps;
	lamboIncY = (KeyFrame[playIndex + 1].lamboPosY - KeyFrame[playIndex].lamboPosY) / i_max_steps;
	lamboIncZ = (KeyFrame[playIndex + 1].lamboPosZ - KeyFrame[playIndex].lamboPosZ) / i_max_steps;
	lamboGiroInc = (KeyFrame[playIndex + 1].lamboGiro - KeyFrame[playIndex].lamboGiro) / i_max_steps;

	puertaIncX = (KeyFrame[playIndex + 1].lamboPosX - KeyFrame[playIndex].lamboPosX) / i_max_steps;
	puertaIncY = (KeyFrame[playIndex + 1].lamboPosY - KeyFrame[playIndex].lamboPosY) / i_max_steps;
	puertaIncZ = (KeyFrame[playIndex + 1].lamboPosZ - KeyFrame[playIndex].lamboPosZ) / i_max_steps;
	puertaGiroInc = (KeyFrame[playIndex + 1].lamboGiro - KeyFrame[playIndex].lamboGiro) / i_max_steps;


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
			posXhomero += homeroIncX; 
			posYhomero += homeroIncY;
			posZhomero += homeroIncZ;

			giroHomero += homeroGiroInc;
			rotPiernaDerHomero += homeroPierDerInc;
			rotPiernaIzqHomero += homeroPierIzqInc;
			movBrazoDerHomero += homeroBrazoDerInc;
			movBrazoIzqHomero += homeroBrazoIzqInc;
			//Marge
			posXmarge += margeIncX;
			posYmarge += margeIncY;
			posZmarge += margeIncZ;

			giroMarge += margeGiroInc;
			rotPiernaDerMarge += margePierDerInc;
			rotPiernaIzqMarge += margePierIzqInc;
			movBrazoDerMarge += margeBrazoDerInc;
			movBrazoIzqMarge += margeBrazoIzqInc;
			//Bart
			posXbart += bartIncX;
			posYbart += bartIncY;
			posZbart += bartIncZ;

			giroBart += bartGiroInc;
			rotPiernaDerBart += bartPierDerInc;
			rotPiernaIzqBart += bartPierIzqInc;
			movBrazoDerBart += bartBrazoDerInc;
			movBrazoIzqBart += bartBrazoIzqInc;
			//Lambo
			posXlambo += lamboIncX;
			posYlambo += lamboIncY;
			posZlambo += lamboIncZ;
			giroLambo += lamboGiroInc;
			//puerta
			posXpuerta += puertaIncX;
			posYpuerta += puertaIncY;
			posZpuerta += puertaIncZ;
			giroPuerta += puertaGiroInc;
			i_curr_steps++;
		}

	}
	
}

void display(	//$$$$$ Funcion display preconfigurada
	Shader shader, Shader skyboxShader, GLuint skybox, 
	Model homerotorso, Model homeropiernaDer, Model homeropiernaIzq,
	Model homerobrazoDer, Model homerobrazoIzq, Model homerocabeza, 
	Model margetorso, Model margepiernaDer, Model margepiernaIzq,
	Model margebrazoDer, Model margebrazoIzq, Model margecabeza,
	Model barttorso, Model bartpiernaDer, Model bartpiernaIzq,
	Model bartbrazoDer, Model bartbrazoIzq, Model bartcabeza,
	Model lambo, Model puerta,
	Model escenario
){
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
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 190.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	shader.setMat4("model", model);
	shader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	shader.setMat4("projection", projection);

	//Escenario
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -100.0f));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial del escenario
	//model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));// <--- tamaño del escenario .. NO ESCALAR
	shader.setMat4("model", model);
	escenario.Draw(shader);
	// ########################################## HOMERO #############################################
	//Homero torso
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de homero
	model = glm::translate(model, glm::vec3(posXhomero, posYhomero, posZhomero));//$$$$$$$$$$$$$$$$$$$$$$$$$$ posicion del modelo en funcion del teclado
	tmp = model = glm::rotate(model, glm::radians(giroHomero), glm::vec3(0.0f, 1.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
	shader.setMat4("model", model);
	homerotorso.Draw(shader);

	//Pierna Der Homero 
	model = glm::translate(tmp, glm::vec3(-0.6f, -1.5f, 0.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(rotPiernaDerHomero), glm::vec3(-1.0f, 0.0f, 0.0f)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro de rotRodIzq
	shader.setMat4("model", model);
	homeropiernaDer.Draw(shader);

	//Pierna Izq Homero
	model = glm::translate(tmp, glm::vec3(0.7f, -1.5f, -0.0f));
	model = glm::rotate(model, glm::radians(rotPiernaIzqHomero), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	homeropiernaIzq.Draw(shader);

	//Brazo derecho Homero
	model = glm::translate(tmp, glm::vec3(-1.0f, 0.6f, 0.0f));
	//model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(movBrazoDerHomero), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	homerobrazoDer.Draw(shader);

	//Brazo izquierdo Homero
	model = glm::translate(tmp, glm::vec3(1.4f, 0.7f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(movBrazoIzqHomero), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	homerobrazoIzq.Draw(shader);

	//Cabeza Homero
	model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
	shader.setMat4("model", model);
	homerocabeza.Draw(shader);

	// ########################################## MARGE #############################################
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de homero
	model = glm::translate(model, glm::vec3(posXmarge, posYmarge, posZmarge));//$$$$$$$$$$$$$$$$$$$$$$$$$$ posicion del modelo en funcion del teclado
	tmp = model = glm::rotate(model, glm::radians(giroMarge), glm::vec3(0.0f, 1.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
	shader.setMat4("model", model);
	margetorso.Draw(shader);

	//Pierna Der Homero 
	model = glm::translate(tmp, glm::vec3(-0.6f, -1.5f, 0.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(rotPiernaDerMarge), glm::vec3(-1.0f, 0.0f, 0.0f)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro de rotRodIzq
	shader.setMat4("model", model);
	margepiernaDer.Draw(shader);

	//Pierna Izq Homero
	model = glm::translate(tmp, glm::vec3(0.7f, -1.5f, -0.0f));
	model = glm::rotate(model, glm::radians(rotPiernaIzqMarge), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	margepiernaIzq.Draw(shader);

	//Brazo derecho Homero
	model = glm::translate(tmp, glm::vec3(-1.4f, 0.6f, 0.0f));
	//model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(movBrazoDerMarge), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	margebrazoDer.Draw(shader);

	//Brazo izquierdo Homero
	model = glm::translate(tmp, glm::vec3(1.4f, 0.6f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(movBrazoIzqMarge), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	margebrazoIzq.Draw(shader);

	//Cabeza Homero
	model = glm::translate(tmp, glm::vec3(0.1f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
	shader.setMat4("model", model);
	margecabeza.Draw(shader);

	// ########################################## BART #############################################
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de homero
	model = glm::translate(model, glm::vec3(posXbart, posYbart, posZbart));//$$$$$$$$$$$$$$$$$$$$$$$$$$ posicion del modelo en funcion del teclado
	tmp = model = glm::rotate(model, glm::radians(giroBart), glm::vec3(0.0f, 1.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
	shader.setMat4("model", model);
	barttorso.Draw(shader);

	//Pierna Der Homero 
	model = glm::translate(tmp, glm::vec3(-0.6f, -1.5f, 0.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	model = glm::rotate(model, glm::radians(rotPiernaDerBart), glm::vec3(-1.0f, 0.0f, 0.0f)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro de rotRodIzq
	shader.setMat4("model", model);
	bartpiernaDer.Draw(shader);

	//Pierna Izq Homero
	model = glm::translate(tmp, glm::vec3(0.7f, -1.5f, -0.0f));
	model = glm::rotate(model, glm::radians(rotPiernaIzqBart), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	bartpiernaIzq.Draw(shader);

	//Brazo derecho Homero
	model = glm::translate(tmp, glm::vec3(-1.4f, 0.7f, 0.0f));
	//model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(movBrazoDerBart), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	bartbrazoDer.Draw(shader);

	//Brazo izquierdo Homero
	model = glm::translate(tmp, glm::vec3(1.4f, 0.6f, 0.0f));
	//model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
	model = glm::rotate(model, glm::radians(movBrazoIzqBart), glm::vec3(-1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	bartbrazoIzq.Draw(shader);

	//Cabeza Homero
	model = glm::translate(tmp, glm::vec3(0.1f, 2.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
	//model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
	shader.setMat4("model", model);
	bartcabeza.Draw(shader);

	//####################################### LAMBO PUERTA ################################
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
	model = glm::translate(model, glm::vec3(posXlambo, posYlambo, posZlambo));//$$$$$$$$$$$$$$$$$$$$$$$$$$ posicion del modelo en funcion del teclado
	/*tmp = */model = glm::rotate(model, glm::radians(giroLambo), glm::vec3(0.0f, 1.0f, 0.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
	shader.setMat4("model", model);
	lambo.Draw(shader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-42.0f, 3.0f, -62.0f));//$$$$$$$$$$$$$$$$$$$$$$$$$$ Posicion inicial de el lambo
	/*tmp = */model = glm::rotate(model, glm::radians(giroPuerta), glm::vec3(0.0f, 0.0f, 1.0)); //$$$$$$$$$$$$$$$$$$$$$$$$$$ giro modelo en funcion del teclado
	shader.setMat4("model", model);
	puerta.Draw(shader);



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

	Model escenario = ((char *)"Models/Escenario/escenario2.obj");

	// Las partes de Homero
	Model torso = ((char *)"Models/Personajes/Homero/torso.obj");
	Model piernaDer = ((char *)"Models/Personajes/Homero/piernaDer.obj");
	Model piernaIzq = ((char *)"Models/Personajes/Homero/piernaIzq.obj");
	Model brazoDer = ((char *)"Models/Personajes/Homero/brazoDer.obj");
	Model brazoIzq = ((char *)"Models/Personajes/Homero/brazoIzq.obj");
	Model cabeza = ((char *)"Models/Personajes/Homero/cabeza.obj");

	// Las partes de Marge
	Model margetorso = ((char *)"Models/Personajes/Marge/torso.obj");
	Model margepiernaDer = ((char *)"Models/Personajes/Marge/piernaDer.obj");
	Model margepiernaIzq = ((char *)"Models/Personajes/Marge/piernaIzq.obj");
	Model margebrazoDer = ((char *)"Models/Personajes/Marge/brazoDer.obj");
	Model margebrazoIzq = ((char *)"Models/Personajes/Marge/brazoIzq.obj");
	Model margecabeza = ((char *)"Models/Personajes/Marge/cabeza.obj");

	// Las partes de Bart
	Model barttorso = ((char *)"Models/Personajes/Bart/torso.obj");
	Model bartpiernaDer = ((char *)"Models/Personajes/Bart/piernaDer.obj");
	Model bartpiernaIzq = ((char *)"Models/Personajes/Bart/piernaIzq.obj");
	Model bartbrazoDer = ((char *)"Models/Personajes/Bart/brazoDer.obj");
	Model bartbrazoIzq = ((char *)"Models/Personajes/Bart/brazoIzq.obj");
	Model bartcabeza = ((char *)"Models/Personajes/Bart/cabeza.obj");

	//Lambo y puerta
	Model lambo = ((char *)"Models/Personajes/Lambo/lambo.obj");
	Model puerta = ((char *)"Models/Personajes/Puerta/puerta.obj");


	//Inicialización de todos los KeyFrames a un valor de cero
	for (int i = 0; i < max_frames; i++)
	{
		KeyFrame[i].homeroPosX			= 0;
		KeyFrame[i].homeroPosY			= 0;
		KeyFrame[i].homeroPosZ			= 0;
		KeyFrame[i].homeroGiro			= 0;
		KeyFrame[i].homeroPiernaDer		= 0;
		KeyFrame[i].homeroPiernaIzq		= 0;
		KeyFrame[i].homeroBrazoDer		= 0;
		KeyFrame[i].homeroBrazoIzq		= 0;

		KeyFrame[i].margePosX			= 0;
		KeyFrame[i].margePosY			= 0;
		KeyFrame[i].margePosZ			= 0;
		KeyFrame[i].margeGiro			= 0;
		KeyFrame[i].margePiernaDer		= 0;
		KeyFrame[i].margePiernaIzq		= 0;
		KeyFrame[i].margeBrazoDer		= 0;
		KeyFrame[i].margeBrazoIzq		= 0;

		KeyFrame[i].bartPosX			= 0;
		KeyFrame[i].bartPosY			= 0;
		KeyFrame[i].bartPosZ			= 0;
		KeyFrame[i].bartGiro			= 0;
		KeyFrame[i].bartPiernaDer		= 0;
		KeyFrame[i].bartPiernaIzq		= 0;
		KeyFrame[i].bartBrazoDer		= 0;
		KeyFrame[i].bartBrazoIzq		= 0;

		KeyFrame[i].lamboPosX = 0;
		KeyFrame[i].lamboPosY = 0;
		KeyFrame[i].lamboPosZ = 0;
		KeyFrame[i].lamboGiro = 0;

		KeyFrame[i].puertaPosX = 0;
		KeyFrame[i].puertaPosY = 0;
		KeyFrame[i].puertaPosZ = 0;
		KeyFrame[i].puertaGiro = 0;
	
	
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
				torso, piernaDer,piernaIzq, brazoDer, brazoIzq, cabeza, 
				margetorso,
				margepiernaDer,
				margepiernaIzq,
				margebrazoDer,
				margebrazoIzq,
				margecabeza,
				barttorso,
				bartpiernaDer,
				bartpiernaIzq,
				bartbrazoDer,
				bartbrazoIzq,
				bartcabeza,
				lambo,
				puerta,
				escenario
		); //$$$$$$$$$$$$$$$$$$$$$$$$$$ carga las texturas, los modelos en el display

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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//Posicion del modelo
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		posXhomero+=2;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		posXhomero-=2;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		posZhomero+= 2;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		posZhomero-= 2;
	
	// Movimiento de sus articulaciones
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		giroHomero+= 2;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		giroHomero-= 2;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotPiernaDerHomero+= 2;
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		rotPiernaDerHomero-= 2;
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotPiernaIzqHomero+= 2;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		rotPiernaIzqHomero-= 2;
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		movBrazoDerHomero+= 2;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		movBrazoDerHomero-= 2;
						//Marge
	//Posicion del modelo
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		posXmarge += 2;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		posXmarge -= 2;
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		posZmarge += 2;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		posZmarge -= 2;

	// Movimiento de sus articulaciones
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		giroMarge += 2;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		giroMarge -= 2;
	}
	if (glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS)
		rotPiernaDerMarge += 2;
	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		rotPiernaDerMarge -= 2;
	if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
		rotPiernaIzqMarge += 2;
	if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
		rotPiernaIzqMarge -= 2;
	if (glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS)
		movBrazoDerMarge += 2;
	if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		movBrazoDerMarge -= 2;

						//Bart
	//Posicion del modelo
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		posXbart += 2;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		posXbart -= 2;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		posZbart += 2;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		posZbart -= 2;

	// Movimiento de sus articulaciones
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		giroBart += 2;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		giroBart -= 2;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		rotPiernaDerBart += 2;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		rotPiernaDerBart -= 2;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		rotPiernaIzqBart += 2;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		rotPiernaIzqBart -= 2;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		movBrazoDerBart += 2;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		movBrazoDerBart -= 2;
				//Lambo
	//Posicion del modelo
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		posXlambo += 2;
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		posXlambo -= 2;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posZlambo += 2;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		posZlambo -= 2;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) 
		giroLambo += 2;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) 
		giroLambo -= 2;

		//Lambo
	//Posicion del modelo
	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) 
		giroPuerta += 2;
	if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) 
		giroPuerta -= 2;
	



	
	//To play KeyFrame animation 
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
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
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		if (FrameIndex < max_frames)
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