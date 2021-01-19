 #include <iostream>
#include <cmath>

//Audio
#include <Windows.h>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "modelAnim.h"

// Audio
#pragma comment(lib, "winmm.lib")

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement(void);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera

Camera  camera(glm::vec3(650.0f, 20.0f, 0.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;


// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(590.0f, 5.0f, 0.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

bool active;


// Deltatime
GLfloat deltaTime = 0.5f;	// Time between current frame and last frame
GLfloat lastFrame = 0.5f;  	// Time of last frame

// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z;


// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(posX,posY,posZ),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0),
	glm::vec3(0,0,0)
};

glm::vec3 LightP1;


int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.frag");


	//Cargar los modelos
	Model ourModelFachada((char*)"Models/CasaSimpson/Fachada.obj");
	Model ourModelSegundoPiso((char*)"Models/CasaSimpson/SegundoPiso.obj");
	Model ourModelPrimerPiso((char*)"Models/CasaSimpson/PrimerPiso.obj");
	Model ExtVentanas((char*)"Models/CasaSimpson/VENTANAS/VENTANASFRENTE/VentanasFrente.obj");
	Model IntVentanas((char*)"Models/CasaSimpson/VENTANAS/VENTANASREGULARES/VentanasRegular.obj");

	//------------------------------Fuera de la casa --------------------------------//
	Model ExtArbol((char*)"Models/CasaSimpson/Fuera/Arbol1.obj");
	Model ExtArbusto((char*)"Models/CasaSimpson/Fuera/Arbusto.obj");
	Model ExtBarda((char*)"Models/CasaSimpson/Fuera/Barda.obj");
	Model ExtBuzon((char*)"Models/CasaSimpson/Fuera/Buzon.obj");
	Model ExtArena((char*)"Models/CasaSimpson/Fuera/CajaArena.obj");
	Model ExtCasaArbol((char*)"Models/CasaSimpson/Fuera/CasaArbol.obj");
	Model ExtPerro((char*)"Models/CasaSimpson/Fuera/CasaPerro.obj");
	Model ExtManguera((char*)"Models/CasaSimpson/Fuera/Manguera.obj");
	Model ExtSuelo((char*)"Models/CasaSimpson/Fuera/Suelo.obj");

	//++++++++++++++++++++++++PLANTA ALTA++++++++++++++++++++++++++++++++++++++++++++//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

	/************************COCINA*****************************************************/
	Model CocinaCenArribaDer((char*)"Models/CasaSimpson/COCINA/cenArribaDer.obj");
	Model CocinaCenArribaIzq((char*)"Models/CasaSimpson/COCINA/cenArribaIzq.obj");
	Model CocinaDerAbajo((char*)"Models/CasaSimpson/COCINA/derAbajo.obj");
	Model CocinaDerArriba((char*)"Models/CasaSimpson/COCINA/derArriba.obj");
	Model CocinaEstufa((char*)"Models/CasaSimpson/COCINA/estufa.obj");
	Model CocinaFregadero((char*)"Models/CasaSimpson/COCINA/Fregadero.obj");
	Model CocinaizqAbajo((char*)"Models/CasaSimpson/COCINA/izqAbajo.obj");
	Model CocinaizqArriba((char*)"Models/CasaSimpson/COCINA/izqArriba.obj");
	Model Cocinalavaplatos((char*)"Models/CasaSimpson/COCINA/lavaplatos.obj");
	Model Cocinalicuadora((char*)"Models/CasaSimpson/COCINA/licuadora.obj");
	Model Cocinarefri((char*)"Models/CasaSimpson/COCINA/refri.obj");
	Model CocinasillaC((char*)"Models/CasaSimpson/COCINA/sillaC.obj");
	Model Cocinamesa((char*)"Models/CasaSimpson/COCINA/mesacocina.obj");

	/***********************************COCHERA***********************************************/
	Model CocheraCaja((char*)"Models/CasaSimpson/COCHERA/caja.obj");
	Model CocherCajaabierta((char*)"Models/CasaSimpson/COCHERA/CajaAbierta.obj");
	Model CocheraEscobaMedieval((char*)"Models/CasaSimpson/COCHERA/EscobaMedieval.obj");
	Model CocheraLatas((char*)"Models/CasaSimpson/COCHERA/Latas.obj");
	Model CocheraLocker((char*)"Models/CasaSimpson/COCHERA/Locker.obj");
	Model CocheraMesaCochera((char*)"Models/CasaSimpson/COCHERA/MesaCochera.obj");
	Model CocheraTablaHerramientas((char*)"Models/CasaSimpson/COCHERA/TablaHerramientas.obj");
	Model CocheraTablaHerramientasPared2((char*)"Models/CasaSimpson/COCHERA/TablaHerramientasPared2.obj");
	
	/**************************************SALA PIANO*************************************************/
	Model CPBanco((char*)"Models/CasaSimpson/CUARTO PIANO/Banco.obj");
	Model CPBurosPiano((char*)"Models/CasaSimpson/CUARTO PIANO/BurosPiano.obj");
	Model CPChimenea((char*)"Models/CasaSimpson/CUARTO PIANO/Chimenea.obj");
	Model CPLamparasDeMesaPiano((char*)"Models/CasaSimpson/CUARTO PIANO/LamparasDeMesaPiano.obj");
	Model CPMasaConPlato((char*)"Models/CasaSimpson/CUARTO PIANO/MasaConPlato.obj");
	Model CPMesaCentro((char*)"Models/CasaSimpson/CUARTO PIANO/MesaCentro.obj");
	Model CPMesaConLampara((char*)"Models/CasaSimpson/CUARTO PIANO/MesaConLampara.obj");
	Model CPMuebleChungo((char*)"Models/CasaSimpson/CUARTO PIANO/MuebleChungo.obj");
	Model CPPiano((char*)"Models/CasaSimpson/CUARTO PIANO/Piano.obj");
	Model CPSillaSalaPiano((char*)"Models/CasaSimpson/CUARTO PIANO/SillaSalaPiano.obj");
	Model CPSillonChico((char*)"Models/CasaSimpson/CUARTO PIANO/SillonChico.obj");
	Model CPSillonGrande((char*)"Models/CasaSimpson/CUARTO PIANO/SillonGrande.obj");
	Model CPSillonTazones((char*)"Models/CasaSimpson/CUARTO PIANO/Tazones.obj");
	Model CPSillonTrofeo1((char*)"Models/CasaSimpson/CUARTO PIANO/Trofeo1.obj");
	Model CPSillonTrofeo3((char*)"Models/CasaSimpson/CUARTO PIANO/Trofeo3.obj");
	Model CPSillonTrofeo2((char*)"Models/CasaSimpson/CUARTO PIANO/Trofeo2.obj");
	Model CPSillonTrofeoS((char*)"Models/CasaSimpson/CUARTO PIANO/Trofeos.obj");

	/********************************************SALA TV*************************************/
	Model STburo((char*)"Models/CasaSimpson/SALA TV/buro.obj");
	Model STcuadropared((char*)"Models/CasaSimpson/SALA TV/cuadropared.obj");
	Model STlamparaMesa((char*)"Models/CasaSimpson/SALA TV/lamparaMesa.obj");
	Model STlamparaPiso((char*)"Models/CasaSimpson/SALA TV/lamparaPiso.obj");
	Model STphone((char*)"Models/CasaSimpson/SALA TV/phone.obj");
	Model STsillon((char*)"Models/CasaSimpson/SALA TV/sillon.obj");
	Model STstereo((char*)"Models/CasaSimpson/SALA TV/stereo.obj");
	Model STtv((char*)"Models/CasaSimpson/SALA TV/tv.obj");
	
	//--------------------------------Cuarto de juegos------------------------------//


	Model JEstante((char*)"Models/CasaSimpson/CUARTOJUEGOS/estante/Estante.obj");
	Model JMesadeTV((char*)"Models/CasaSimpson/CUARTOJUEGOS/mesa_de_tv/MesaDeTv.obj");
	Model JMuebleJarrones((char*)"Models/CasaSimpson/CUARTOJUEGOS/mueble_con_jarrones/MuebleJarrones.obj");
	Model JSillonJuegos((char*)"Models/CasaSimpson/CUARTOJUEGOS/sillon/Sillon.obj");
	Model JTvmaggie((char*)"Models/CasaSimpson/CUARTOJUEGOS/tvmaggie/Tv.obj");
	Model JCajonChico1((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedchico/CajonChico1.obj");
	Model JCajonChico2((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedchico/CajonChico2.obj");
	Model JCajonChico3((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedchico/CajonChico3.obj");
	Model JCajonChico4((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedchico/CajonChico4.obj");
	Model JCajonChico5((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedchico/CajonChico5.obj");
	Model JCajonChico6((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedchico/CajonChico6.obj");
	Model JCajonChico7((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedchico/CajonChico7.obj");
	Model JCajonChico8((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedchico/CajonChico8.obj");
	Model JCajonGrande1((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedgrande/CajonGrande1.obj");
	Model JCajonGrande2((char*)"Models/CasaSimpson/CUARTOJUEGOS/cajonparedgrande/CajonGrande2.obj");

	//--------------------------------Pasillo Planta baja------------------------------//

	Model PEscritorioPasillo((char*)"Models/CasaSimpson/PASILLOPB/Escritorio/Escritorio.obj");
	Model PReloj((char*)"Models/CasaSimpson/PASILLOPB/Reloj/Reloj.obj");

	//---------------------Comedor------------------------------------------------------//

	Model ComeBufetera((char*)"Models/CasaSimpson/COMEDOR/bufetera/bufetera.obj");
	Model ComeCuadro((char*)"Models/CasaSimpson/COMEDOR/cuadro/cuadro.obj");
	Model ComeMesa((char*)"Models/CasaSimpson/COMEDOR/mesa/mesa.obj");
	Model ComeSilla((char*)"Models/CasaSimpson/COMEDOR/silla/silla.obj");



	//--------------------------------Habitación de Bart------------------------------//
	Model BartCama((char*)"Models/CasaSimpson/BART/cama.obj");
	Model BartBuroes((char*)"Models/CasaSimpson/BART/Buroes.obj");
	Model BartLibrero((char*)"Models/CasaSimpson/BART/Librero.obj");
	Model BartEscritorio((char*)"Models/CasaSimpson/BART/Escritorio.obj");
	Model BartEspejo((char*)"Models/CasaSimpson/BART/Tocador.obj");
	Model BartPizarron((char*)"Models/CasaSimpson/BART/Pizarron.obj");
	Model BartRepisa((char*)"Models/CasaSimpson/BART/Repisa.obj");
	Model BartSilla((char*)"Models/CasaSimpson/BART/Silla.obj");

	//--------------------------------Habitación de Lisa------------------------------//
	Model LisaCama((char*)"Models/CasaSimpson/LISA/Cama.obj");
	Model LisaCuadro((char*)"Models/CasaSimpson/LISA/Cuadro.obj");
	Model LisaAtril((char*)"Models/CasaSimpson/LISA/Atril.obj");
	Model LisaBuroes((char*)"Models/CasaSimpson/LISA/Buroes.obj");
	Model LisaEspejo((char*)"Models/CasaSimpson/LISA/Espejo.obj");
	Model LisaEstante((char*)"Models/CasaSimpson/LISA/Estante.obj");
	Model LisaLampara((char*)"Models/CasaSimpson/LISA/Lampara.obj");
	Model LisaLibrero((char*)"Models/CasaSimpson/LISA/Librero.obj");
	Model LisaSilla((char*)"Models/CasaSimpson/LISA/Silla.obj");

	/***************************************BAÑO HOMERO******************************************/
	Model Ducha((char*)"Models/CasaSimpson/HOMERO-MARGE/BAÑO/Ducha.obj");
	Model WC((char*)"Models/CasaSimpson/HOMERO-MARGE/BAÑO/WC.obj");
	Model MueblePared((char*)"Models/CasaSimpson/HOMERO-MARGE/BAÑO/MueblePared.obj");
	Model Mueble((char*)"Models/CasaSimpson/HOMERO-MARGE/BAÑO/Mueble.obj");
	Model Lavavo((char*)"Models/CasaSimpson/HOMERO-MARGE/BAÑO/Lavavo.obj");
	Model Regadera((char*)"Models/CasaSimpson/HOMERO-MARGE/BAÑO/Regadera.obj");
	Model Espejo((char*)"Models/CasaSimpson/HOMERO-MARGE/BAÑO/Espejo.obj");
	Model Toallero((char*)"Models/CasaSimpson/HOMERO-MARGE/BAÑO/Toallero.obj");

	/************************CUARTO HOMERO Y MARGE*************************************************/

	Model HMCama((char*)"Models/CasaSimpson/HOMERO-MARGE/Cama.obj");
	Model HMBuroReloj((char*)"Models/CasaSimpson/HOMERO-MARGE/BuroReloj.obj");
	Model HMBuroLampara((char*)"Models/CasaSimpson/HOMERO-MARGE/BuroLampara.obj");
	Model HMTocador((char*)"Models/CasaSimpson/HOMERO-MARGE/Tocador.obj");
	Model HMBanco((char*)"Models/CasaSimpson/HOMERO-MARGE/Banco.obj");
	Model HMEspejo((char*)"Models/CasaSimpson/HOMERO-MARGE/Espejo.obj");
	Model HMSillon((char*)"Models/CasaSimpson/HOMERO-MARGE/Sillon.obj");
	Model HMComoda((char*)"Models/CasaSimpson/HOMERO-MARGE/Comoda.obj");
	Model HMAlmohada((char*)"Models/CasaSimpson/HOMERO-MARGE/Almohada.obj");
	
	/**************************************MAGGIE***********************************************/
	Model MaggieCuna((char*)"Models/CasaSimpson/MAGGIE/Cuna.obj");
	Model MaggieCorral((char*)"Models/CasaSimpson/MAGGIE/Corralito.obj");
	Model MaggieJuguetero((char*)"Models/CasaSimpson/MAGGIE/Juguetero.obj");
	Model MaggieBuro((char*)"Models/CasaSimpson/MAGGIE/Buro.obj");
	Model MaggieColumpio((char*)"Models/CasaSimpson/MAGGIE/Columpio.obj");
	Model MaggieCesto((char*)"Models/CasaSimpson/MAGGIE/Cesto.obj");

	//--------------------------------Baño Planta Alta------------------------------//

	Model BanoBote((char*)"Models/CasaSimpson/BañoPA/Bote.obj");
	Model BanoEspejo((char*)"Models/CasaSimpson/BañoPA/Espejo.obj");
	Model BanoEstante((char*)"Models/CasaSimpson/BañoPA/Estante.obj");
	Model BanoLavabo((char*)"Models/CasaSimpson/BañoPA/Lavabo.obj");
	Model BanoTina((char*)"Models/CasaSimpson/BañoPA/Tina.obj");
	Model BanoWC((char*)"Models/CasaSimpson/BañoPA/WC.obj");


	// Cargar modelos de animación
	ModelAnim animacionHomero("Animaciones/Homero/Homer_bones.fbx");
	animacionHomero.initShaders(animShader.Program);

	ModelAnim animacionBart("Animaciones/Bart/bart_bones.fbx");
	animacionBart.initShaders(animShader.Program);

	ModelAnim animacionLisa("Animaciones/Lisa/Lisa_bones.fbx");
	animacionLisa.initShaders(animShader.Program);


	ModelAnim animacionSnail("Animaciones/Snail/snail.fbx");
	animacionSnail.initShaders(animShader.Program);

	ModelAnim animacionMarge("Animaciones/Marge/Marge_Bones.fbx");
	animacionMarge.initShaders(animShader.Program);

	ModelAnim animacionToy("Animaciones/Toy_Ball/Toy_Ball.fbx");
	animacionToy.initShaders(animShader.Program);


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



	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1,&skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices),&skyboxVertices,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);

	// Cargar SKYB0X
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/Simpson/right.jpg");
	faces.push_back("SkyBox/Simpson/left.jpg");
	faces.push_back("SkyBox/Simpson/top.jpg");
	faces.push_back("SkyBox/Simpson/bottom.jpg");
	faces.push_back("SkyBox/Simpson/back.jpg");
	faces.push_back("SkyBox/Simpson/front.jpg");

	
	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);


		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/

		glm::mat4 tmp = glm::mat4(1.0f); //Temp



		//Carga de modelos
//-------------------------------------------Fachada y pisos -------------------------------------------------------//
		glm::mat4 model(1);

		//Fachada
		view = camera.GetViewMatrix();
		//model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		//model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ourModelFachada.Draw(lightingShader);


		//Segundo Piso
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ourModelSegundoPiso.Draw(lightingShader);

		//Primer Piso
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ourModelPrimerPiso.Draw(lightingShader);

		//Ventanas Exteriores
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtVentanas.Draw(lightingShader);

		//Ventanas Interiores
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		IntVentanas.Draw(lightingShader);


		

		//---------------------Fuera de la casa------------------------------------------------------//

		//Arbol
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtArbol.Draw(lightingShader);

		//Arbusto
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtArbusto.Draw(lightingShader);

		//Barda
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtBarda.Draw(lightingShader);

		//Buzon
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtBuzon.Draw(lightingShader);

		//Arena
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtArena.Draw(lightingShader);

		//CasaArbol
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtCasaArbol.Draw(lightingShader);

		//CasaPerro
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtPerro.Draw(lightingShader);

		//Manguera
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtManguera.Draw(lightingShader);

		//Suelo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ExtSuelo.Draw(lightingShader);

//++++++++++++++++++++++++++++++++++++++++PLANTA BAJA +++++++++++++++++++++++++++++++++++++++++++++++++++//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

	/*****************************COCINA******************************************/
		//CenArribaDer
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinaCenArribaDer.Draw(lightingShader);
		
		//CenArribaIzq
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinaCenArribaIzq.Draw(lightingShader);

		//DerAbajo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinaDerAbajo.Draw(lightingShader);
		
		//DerArriba
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinaDerArriba.Draw(lightingShader);

		//Estufa
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinaEstufa.Draw(lightingShader);
		
		//Fregadero
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinaFregadero.Draw(lightingShader);

		//CocinaIzqAbajo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinaizqAbajo.Draw(lightingShader);
		
		//CocinaIzqArriba
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinaizqArriba.Draw(lightingShader);

		//Lava platos
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cocinalavaplatos.Draw(lightingShader);

		//Licuadora
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cocinalicuadora.Draw(lightingShader);
		
		//Refri
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cocinarefri.Draw(lightingShader);
		
		//SillaC
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocinasillaC.Draw(lightingShader);
		
		//Mesa
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cocinamesa.Draw(lightingShader);

//**********************************COCHERA***************************************************/
		//Caja
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocheraCaja.Draw(lightingShader);

		//Caja Abierta
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocherCajaabierta.Draw(lightingShader);

		//Escoba
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocheraEscobaMedieval.Draw(lightingShader);

		//Latas
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocheraLatas.Draw(lightingShader);

		//Locker
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocheraLocker.Draw(lightingShader);
		
		//Mesa
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocheraMesaCochera.Draw(lightingShader);
		
		//Tabla Herraientas
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocheraTablaHerramientas.Draw(lightingShader);
		
		//Tabla Herraientas pared
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CocheraTablaHerramientasPared2.Draw(lightingShader);
		
	/**********************************************CUARTO PIANO***********************************/
		
		//Banco
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPBanco.Draw(lightingShader);

		//Buros
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPBurosPiano.Draw(lightingShader);
		
		//Chimenea
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPChimenea.Draw(lightingShader);

		//Lamparas
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPLamparasDeMesaPiano.Draw(lightingShader);
		
		//Mesa
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPMasaConPlato.Draw(lightingShader);

		//Mesa Centro
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPMesaCentro.Draw(lightingShader);

		//Mesa Lampara
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPMesaConLampara.Draw(lightingShader);

		
		//Mueble
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPMuebleChungo.Draw(lightingShader);

		//Piano
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPPiano.Draw(lightingShader);

		//Silla
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPSillaSalaPiano.Draw(lightingShader);

		//Sillon chico
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPSillonChico.Draw(lightingShader);

		//Sillon grande
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPSillonGrande.Draw(lightingShader);

		//Sillon tazones
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPSillonTazones.Draw(lightingShader);

		//Trofeos
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPSillonTrofeo1.Draw(lightingShader);

		//Trofeo 3
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPSillonTrofeo3.Draw(lightingShader);

		//Trofeo 2
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPSillonTrofeo2.Draw(lightingShader);

		//Trofeos
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CPSillonTrofeoS.Draw(lightingShader);

	/******************************************SALA TV **************************************/
	
		//Buro
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		STburo.Draw(lightingShader);


		//Cuadros pared
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		STcuadropared.Draw(lightingShader);


		//Lampara Mesa
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		STlamparaMesa.Draw(lightingShader);


		//Lampara Piso
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		STlamparaPiso.Draw(lightingShader);


		//Telefono
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		STphone.Draw(lightingShader);


		//Sillon
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		STsillon.Draw(lightingShader);

		//Stereo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		STstereo.Draw(lightingShader);

		//TV
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		STtv.Draw(lightingShader);

		//---------------------Cuarto de juegos------------------------------------------------------//


				//Estante
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JEstante.Draw(lightingShader);
		//Mesa
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JMesadeTV.Draw(lightingShader);
		//Mueble Jarrones
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JMuebleJarrones.Draw(lightingShader);
		//Mesa
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JSillonJuegos.Draw(lightingShader);
		//TV
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JTvmaggie.Draw(lightingShader);

		//Cajones
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonChico1.Draw(lightingShader);
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonChico2.Draw(lightingShader);
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonChico3.Draw(lightingShader);
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonChico4.Draw(lightingShader);
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonChico5.Draw(lightingShader);
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonChico6.Draw(lightingShader);
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonChico7.Draw(lightingShader);
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonChico8.Draw(lightingShader);

		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonGrande1.Draw(lightingShader);

		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		JCajonGrande2.Draw(lightingShader);

		//---------------------Pasillo------------------------------------------------------/
				//Escritorio
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PEscritorioPasillo.Draw(lightingShader);
		//Reloj
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PReloj.Draw(lightingShader);

		//---------------------Comedor------------------------------------------------------/

			//Bufetera
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ComeBufetera.Draw(lightingShader);
		//Cuadro
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ComeCuadro.Draw(lightingShader);
		//Mesa
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ComeMesa.Draw(lightingShader);
		//Silla
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ComeSilla.Draw(lightingShader);

	
//++++++++++++++++++++++++++++++++++++++++++++++++++++PLANTA ALTA+++++++++++++++++++++++++++++++++++++++//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//


//---------------------Habitación de Bart------------------------------------------------------//

		//Cama
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BartCama.Draw(lightingShader);

		//Buroes
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BartBuroes.Draw(lightingShader);

		//Librero
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BartLibrero.Draw(lightingShader);


		//Escritorio
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BartEscritorio.Draw(lightingShader);

		//Espejo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BartEspejo.Draw(lightingShader);

		//Pizarron
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BartPizarron.Draw(lightingShader);

		//Espejo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BartRepisa.Draw(lightingShader);

		//Pizarron
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BartSilla.Draw(lightingShader);



//---------------------Habitación de Lisa------------------------------------------------------//


		//Cama
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaCama.Draw(lightingShader);

		//Cama
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaCuadro.Draw(lightingShader);

		//Buroes
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaBuroes.Draw(lightingShader);

		//Librero
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaLibrero.Draw(lightingShader);


		//Espejo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaEspejo.Draw(lightingShader);


		//Atril
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaAtril.Draw(lightingShader);

		//Estante
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaEstante.Draw(lightingShader);

		//Lampara
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaLampara.Draw(lightingShader);

		//Silla
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LisaSilla.Draw(lightingShader);

	/*************************************************BAÑO DE HOMERO********************************************/

		///*Ducha*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ducha.Draw(lightingShader);

		/*Regadera*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Regadera.Draw(lightingShader);

		/*Lavavo*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lavavo.Draw(lightingShader);

		/*WC*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		WC.Draw(lightingShader);

		/*Espejo*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Espejo.Draw(lightingShader);

		/*Mueble Pared*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MueblePared.Draw(lightingShader);

		/*Mueble*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mueble.Draw(lightingShader);

		/*Toallero*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Toallero.Draw(lightingShader);

	/**************************************HOMERO-MARGE*******************************************************************/
		/*Cama*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMCama.Draw(lightingShader);

		/*Almohada*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMAlmohada.Draw(lightingShader);

		/*Tocador*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMTocador.Draw(lightingShader);

		/*Banco*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMBanco.Draw(lightingShader);

		/*Buro Lampara*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMBuroLampara.Draw(lightingShader);

		/*Buro Reloj*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMBuroReloj.Draw(lightingShader);

		/*Comoda*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMComoda.Draw(lightingShader);

		/*Sillon*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMSillon.Draw(lightingShader);

		/*Espejo*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HMEspejo.Draw(lightingShader);


	/*************************************MAGGIE**************************************************************/
		/*Cuna*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MaggieCuna.Draw(lightingShader);

		/*Corralito*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MaggieCorral.Draw(lightingShader);

		/*Buro*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MaggieBuro.Draw(lightingShader);

		/*Columpio*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MaggieColumpio.Draw(lightingShader);

		/*Cesto*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MaggieCesto.Draw(lightingShader);

		/*Juguetero*/
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MaggieJuguetero.Draw(lightingShader);

		//---------------------Baño Planta Alta------------------------------------------------------//


		//Bote
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BanoBote.Draw(lightingShader);

		//Espejo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BanoEspejo.Draw(lightingShader);

		//Estante
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BanoEstante.Draw(lightingShader);

		//Lavabo
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BanoLavabo.Draw(lightingShader);


		//Tina
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BanoTina.Draw(lightingShader);


		//WC
		view = camera.GetViewMatrix();
		//model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BanoWC.Draw(lightingShader);

		

		glBindVertexArray(0);



		/*_______________________________Personaje Animado___________________________*/ 
		animShader.Use();
		modelLoc = glGetUniformLocation(animShader.Program, "model");
		viewLoc = glGetUniformLocation(animShader.Program, "view");
		projLoc = glGetUniformLocation(animShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(glGetUniformLocation(animShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(animShader.Program, "material.shininess"), 90.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(animShader.Program, "light.direction"),0.0f, 0.0f, 0.0f);
		view = camera.GetViewMatrix();

		//HOMERO
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(PosIni.x, PosIni.y - 5.0f, PosIni.z - 15.0f));
		model = glm::scale(model, glm::vec3(0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		animacionHomero.Draw(animShader);
		glBindVertexArray(0);

		//BART
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(PosIni.x, PosIni.y - 5.0f, PosIni.z - 30.0f));
		model = glm::scale(model, glm::vec3(0.1f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		animacionBart.Draw(animShader);
		glBindVertexArray(0);

		//LISA
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(PosIni.x, PosIni.y - 5.0f, PosIni.z - 45.0f));
		model = glm::scale(model, glm::vec3(0.1f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		animacionLisa.Draw(animShader);
		glBindVertexArray(0);


		//MARGE
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(PosIni.x, PosIni.y + 0.7f, PosIni.z - 60.0f));
		model = glm::scale(model, glm::vec3(0.05f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		animacionMarge.Draw(animShader);
		glBindVertexArray(0);


		//TOY BALL
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(PosIni.x - 55.0f, PosIni.y - 3.0f, PosIni.z + 60.f));
		model = glm::scale(model, glm::vec3(0.001f));	// it's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		animacionToy.Draw(animShader);
		glBindVertexArray(0);

		//CARACOL
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(PosIni.x - 45.0f, PosIni.y - 5.05f, PosIni.z + 20.0f));
		model = glm::scale(model, glm::vec3(0.0004f));	// it's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		animacionSnail.Draw(animShader);
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

	
		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_E] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime*2);

	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime*2);


	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime*2);


	}

	if (keys[GLFW_KEY_F] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime*2);
	}

	if (keys[GLFW_KEY_1])
		PlaySound(TEXT("Sound/doh.wav"), NULL, SND_ASYNC | SND_FILENAME);

	if (keys[GLFW_KEY_2])
		PlaySound(TEXT("Sound/burp.wav"), NULL, SND_ASYNC | SND_FILENAME);

	if (keys[GLFW_KEY_3])
		PlaySound(TEXT("Sound/bartlaf.wav"), NULL, SND_ASYNC | SND_FILENAME);

	if (keys[GLFW_KEY_4])
		PlaySound(TEXT("Sound/organized.wav"), NULL, SND_ASYNC | SND_FILENAME);

	if (keys[GLFW_KEY_5])
		PlaySound(TEXT("Sound/Simpsons.wav"), NULL, SND_ASYNC | SND_FILENAME);

	if (keys[GLFW_KEY_A])
		PlaySound(TEXT("Sound/hidden.wav"), NULL, SND_ASYNC | SND_FILENAME);

	if (keys[GLFW_KEY_O])
		PlaySound(NULL, 0, 0);

}