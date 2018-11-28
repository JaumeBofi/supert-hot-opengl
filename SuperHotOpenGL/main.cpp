#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/freeglut.h>


#include <settings.h>
#include <shader.h>
#include <model.h>
#include <camera.h>
#include "Bala.h"
#include "Enemy.h"
#include <list>

// Settings
Settings settings = Settings();
Shader* mainShader;
Model* model;

//Pistola
GLfloat pistolax;
GLfloat pistolay;
GLfloat pistolaz;
Model* modelPistola;
GLfloat recoil = 0.0f;
bool hasFired = false;
bool goingDown = false;

//Balas
std::list<Bala> listaBalas;
Model* modelBala;

//Test Enemy
Model* modelEnemy;
Enemy enemy1 = Enemy(glm::vec3(-0.075f, -0.0430f, 0.0f), glm::vec3(-0.075f, -0.040f, 0.0f));
Enemy enemy2 = Enemy(glm::vec3(-0.103743, -0.0430f, 0.149888), glm::vec3(-0.103743, -0.040f, 0.149888));

// Camera
Camera camera(glm::vec3(-0.07f, -0.035449f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool prueba = false;

// Timing
int oldTimeSinceStart = 0;
float deltaTime = 0.01f;	// time between current frame and last frame

void display() {
	glClearColor(1.0, 0.0, 0.0, 0.1);
	glClear(GL_COLOR_BUFFER_BIT);

	// don't forget to enable shader before setting uniforms
	mainShader->use();
	int a = 4;

	glutSwapBuffers();
}

bool init_resources()
{
	printf("%s", (settings.ShadersDirectory() + "super_hot.vs").c_str());
	mainShader = new Shader((settings.ShadersDirectory() + "super_hot.vs").c_str(), (settings.ShadersDirectory() + "super_hot.fs").c_str());
	model = new Model("Arc170.obj");
	model->ComputeData();	
	modelPistola = new Model("deagle.obj"); 
	modelPistola->ComputeData();

	modelBala = new Model("bowlingball.obj");
	modelBala->ComputeData();

	modelEnemy = new Model("mercenary.obj");
	modelEnemy->ComputeData();

	
	return true;	
}

void onDisplay() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// don't forget to enable shader before setting uniforms
	mainShader->use();

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.001f, 200.0f);
	glm::mat4 view = camera.GetViewMatrix();
	mainShader->setMat4("projection", projection);
	mainShader->setMat4("view", view);
	mainShader->setVec3("cam_position", camera.Position);




	// render the loaded model
	glm::mat4 mod =				
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(model->scale, model->scale, model->scale)) *		
		glm::translate(glm::mat4(1.0f), glm::vec3(-model->centroid.x, -model->centroid.y, -model->centroid.z)); // translate it down so it's at the center of the scene
	;	// it's a bit too big for our scene, so scale it down
	glm::mat3 mat_inv_transp = glm::transpose(glm::inverse(glm::mat3(mod)));
	mainShader->setMat3("m_3x3", mat_inv_transp);
	mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
	mainShader->setFloat("mat_s", 100);

	mainShader->setMat4("model", mod);
	model->Draw(*mainShader);
	printf("%f posicion camara x\n", camera.Position.x);
	printf("%f posicion camara y\n", camera.Position.y);
	printf("%f posicion camara z\n", camera.Position.z);
	//printf("%f recoil \n", recoil);
	if (hasFired)printf("hasFired= True\n");
	else printf("hasFired= False\n");
	if (goingDown)printf("goingDown= True\n");
	else printf("goingDown= False\n");


	GLfloat alternativeYaw = camera.Yaw *-1;

	glm::mat4 modPistola=
		glm::translate(glm::mat4(1.0f), glm::vec3(pistolax, pistolay, pistolaz)) * //llevar pistola junto a la posicion de la camara
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(recoil), camera.Front) *
		glm::rotate(glm::mat4(1.0f), glm::radians(alternativeYaw), glm::vec3(0.0f, 1.0f, 0.0f)) * //pistola sigue Yaw de la camara
		glm::rotate(glm::mat4(1.0f), glm::radians(camera.Pitch), glm::vec3(1.0f, 0.0f, 0.0f))* //pistola sigue pitch de la camara
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0025f, 0.0f))*  //para que la pistola baje a un nivel aceptable
		glm::rotate(glm::mat4(1.0f), glm::radians(-180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * //para que el modelo no aparezca de cabeza
		glm::scale(glm::mat4(1.0f), glm::vec3(0.0005f, 0.0005f, 0.0005f)) //para escalarlo a un tamaño realista
		 ;	// it's a bit too big for our scene, so scale it down
	glm::mat3 mat_inv_transpPistola = glm::transpose(glm::inverse(glm::mat3(modPistola)));
	mainShader->setMat3("m_3x3", mat_inv_transpPistola);
	mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
	mainShader->setFloat("mat_s", 100);

	mainShader->setMat4("model", modPistola);
	modelPistola->Draw(*mainShader);


	


	std::list<Bala>::iterator bala;
	for (bala = listaBalas.begin(); bala != listaBalas.end(); ++bala) {
		//printf("BALA");
		glm::mat4 modBala = bala->render();
		glm::mat3 mat_inv_transp2 = glm::transpose(glm::inverse(glm::mat3(modBala)));
		mainShader->setMat3("m_3x3", mat_inv_transp2);
		mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
		mainShader->setFloat("mat_s", 100);

		mainShader->setMat4("model", modBala);
		modelBala->Draw(*mainShader);;
	}


	glm::mat4 modEnemy = enemy1.render(modelPistola, modelBala, mainShader, pistolax, pistolaz);

	glm::mat3 mat_inv_transpEnemy = glm::transpose(glm::inverse(glm::mat3(modEnemy)));
	mainShader->setMat3("m_3x3", mat_inv_transpEnemy);
	mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
	mainShader->setFloat("mat_s", 100);

	mainShader->setMat4("model", modEnemy);
	modelEnemy->Draw(*mainShader);

	glm::mat4 modEnemy2 = enemy2.render(modelPistola, modelBala, mainShader, pistolax, pistolaz);
	glm::mat3 mat_inv_transpEnemy2 = glm::transpose(glm::inverse(glm::mat3(modEnemy2)));
	mainShader->setMat3("m_3x3", mat_inv_transpEnemy2);
	mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
	mainShader->setFloat("mat_s", 100);

	mainShader->setMat4("model", modEnemy2);
	modelEnemy->Draw(*mainShader);

/*	for (Bala bala : listaBalas) {
		printf("BALA");
		glm::mat4 modBala = bala.render();
		glm::mat3 mat_inv_transp2 = glm::transpose(glm::inverse(glm::mat3(modBala)));
		mainShader->setMat3("m_3x3", mat_inv_transp2);
		mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
		mainShader->setFloat("mat_s", 100);

		mainShader->setMat4("model", modBala);
		modelPistola->Draw(*mainShader);
	}*/


	glutSwapBuffers();
}

void mouseMovement(int xpos, int ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
	

	glutPostRedisplay();
}
void onClick(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		Bala bala = Bala(camera.Position,glm::vec3(0,0,0),camera.Front,true,true,true);
		listaBalas.push_back(bala);
		enemy1.addBullet(camera.Position);
		enemy2.addBullet(camera.Position);
		//printf("click");
		hasFired = true;
		break;
	}
	glutPostRedisplay();
}

void keyboardInput(unsigned char keycode, int x, int y) {
	    if (keycode == 'q') {
		    prueba = !prueba;
	    }
		if (prueba) {
			//printf("%d\n", deltaTime);
			switch (keycode)
			{
			case 's': // Escape key
				camera.ProcessKeyboardTest(BACKWARD, deltaTime);
				break;
			case 'w': // Escape key
				camera.ProcessKeyboardTest(FORWARD, deltaTime);
				break;
			case 'a': // Escape key
				camera.ProcessKeyboardTest(LEFT, deltaTime);
				break;
			case 'd': // Escape key
				camera.ProcessKeyboardTest(RIGHT, deltaTime);
				break;
			case 27: // Escape key			
				exit(EXIT_SUCCESS);
				break;
			}
			
		}
		else  {
			printf("%d\n", deltaTime);
			switch (keycode)
			{
			case 's': // Escape key
				camera.ProcessKeyboard(BACKWARD, deltaTime);
				break;
			case 'w': // Escape key
				camera.ProcessKeyboard(FORWARD, deltaTime);
				break;
			case 'a': // Escape key
				camera.ProcessKeyboard(LEFT, deltaTime);
				break;
			case 'd': // Escape key
				camera.ProcessKeyboard(RIGHT, deltaTime);
				break;
			case 27: // Escape key			
				exit(EXIT_SUCCESS);
				break;
			}
			
		}
		pistolax = camera.Position.x;
		pistolay = camera.Position.y;
		pistolaz = camera.Position.z;
	
		glutPostRedisplay();
	
	
	
	
	
	
}

float t = 0;
void onIdle()
{
	/*int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);	
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;*/
	t = t + 0.000002;

	std::list<Bala>::iterator it;
	for (it = listaBalas.begin(); it != listaBalas.end(); ++it) {
		it->actualizarPosicion(t);
		//std::cout << it->;

		///manejar luego de otra forma - temporal hasta acabar el ciclo :) 
		enemy1.updateBulletsPosition(t);
		enemy2.updateBulletsPosition(t);
	}



	if (hasFired) {

		if (goingDown) {
			recoil -= 4.0f;
			if (recoil <= 0.0f) {
				hasFired = false;
				goingDown = false;

			}


		}
		else {
			recoil += 4.0f;
			//printf("%f recoil \n", recoil);
			if (recoil >= 12.0f) goingDown = true;
		}
	}

	glutPostRedisplay();
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(settings.ScrWidth(), settings.ScrHeight());
	glutCreateWindow("SUPERHOT-GL");

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		std::cout << "Error GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!GLEW_VERSION_2_0) {
		std::cout << "No hay version 2.0" << std::endl;
		exit(EXIT_FAILURE);
	}

	init_resources();		

	//camera.SetLookPoint(glm::vec3(0.0f, 0.0f, 0.0f));
	glutDisplayFunc(onDisplay);	
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutKeyboardFunc(keyboardInput);
	glutPassiveMotionFunc(mouseMovement);
	glutMouseFunc(onClick);
	glutIdleFunc(onIdle);
	glutMainLoop();

	return EXIT_SUCCESS;
}

