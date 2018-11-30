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
#include <player.h>
#include <ComplexPhysicsObject.h>
#include "coldet.h"


// Settings
Settings settings = Settings();
Shader* mainShader;
Model* sceneBound;
Model* scene;


// Camera
Camera camera(glm::vec3(-0.07f, -0.035449f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool prueba = false;


// Player
glm::vec3 initialPosition = glm::vec3(-0.07f, -0.035449f, 0.0f);
Player player = Player(initialPosition, &camera);

// Timing
int oldTimeSinceStart = 0;
float deltaTime = 0.01f;	// time between current frame and last frame


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
vector<Enemy*> enemies;

// Colision Detection


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
	mainShader = new Shader((settings.ShadersDirectory() + "super_hot.vs").c_str(), (settings.ShadersDirectory() + "super_hot.fs").c_str());
	sceneBound = new Model("Cube.obj");
	sceneBound->ComputeData();
	scene = new Model("Arc170.obj");
	scene->ComputeData();
	scene->ComputeCollisionModel();
	glm::mat4 mod =
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(scene->scale, scene->scale, scene->scale)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(-scene->centroid.x, -scene->centroid.y, -scene->centroid.z)); // translate it down so it's at the center of the scene
	;	// it's a bit too big for our scene, so scale it down
	scene->UpdateCollisionModel(mod);	

	player.Mesh(new Model("Cube.obj"));
	player.Mesh()->ComputeData();
	player.Mesh()->ComputeCollisionModel();
	player.AddWeapon(new Weapon("deagle.obj","bowlingball.obj", 10));


	Enemy* enemy1 = new Enemy("mercenary.obj", new Weapon("deagle.obj", "bowlingball.obj", 10), glm::vec3(-0.239149f, -0.0430f, -0.022f));
	Enemy* enemy2 = new Enemy("mercenary.obj", new Weapon("deagle.obj", "bowlingball.obj", 10), glm::vec3(-0.103743, -0.0430f, 0.149888));
	Enemy* enemy3 = new Enemy("mercenary.obj", new Weapon("deagle.obj", "bowlingball.obj", 10), glm::vec3(-0.029067, -0.0430f, -0.107453), true);

	enemies.push_back(enemy1);
	enemies.push_back(enemy2);
	enemies.push_back(enemy3);
		
	camera.Position = initialPosition;


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
		glm::scale(glm::mat4(1.0f), glm::vec3(scene->scale, scene->scale, scene->scale)) *
		glm::translate(glm::mat4(1.0f), glm::vec3(-scene->centroid.x, -scene->centroid.y, -scene->centroid.z)); // translate it down so it's at the center of the scene
	;	// it's a bit too big for our scene, so scale it down


	glm::mat3 mat_inv_transp = glm::transpose(glm::inverse(glm::mat3(mod)));
	mainShader->setMat3("m_3x3", mat_inv_transp);
	mainShader->setVec3("mat_specular", glm::vec3(1.0, 1.0, 1.0));
	mainShader->setFloat("mat_s", 100);

	mainShader->setMat4("model", mod);	
	scene->Draw(*mainShader);
	mod = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	mainShader->setMat4("model", mod);
	sceneBound->Draw(*mainShader);

	player.UpdatePosition(scene);
	player.GetCurrentWeapon()->render(mainShader);	
	//player.render(mainShader);
	
	vector<Bullet*> bulletsHit;
		
	std::vector<Bullet*>::iterator bullet;
	for (bullet = player.GetCurrentWeapon()->CurrentBullets()->begin(); bullet != player.GetCurrentWeapon()->CurrentBullets()->end(); ++bullet) {
		Enemy* enemyHit = NULL;
		std::vector<Enemy*>::iterator enemy;
		for (enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
			if ((*enemy)->Mesh()->checkColisions((*bullet)->Mesh()))
			{
				enemyHit = (*enemy);
				break;
			}
		}
		
		if (enemyHit != NULL)
		{
			bulletsHit.push_back((*bullet));
			enemyHit->setDead();
		}
		else
		{
			if (player.hasMoved)
			{
				(*bullet)->UpdatePosicion();
			}
			(*bullet)->render(mainShader);
		}
	}
	
	for (bullet = bulletsHit.begin(); bullet != bulletsHit.end(); ++bullet) {
		player.GetCurrentWeapon()->CurrentBullets()->erase(std::remove(player.GetCurrentWeapon()->CurrentBullets()->begin(), player.GetCurrentWeapon()->CurrentBullets()->end(), *bullet), player.GetCurrentWeapon()->CurrentBullets()->end());
	}	

	std::vector<Enemy*>::iterator enemy;
	for (enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
		Weapon* weapon = (*enemy)->CurrentWeapon();
		std::vector<Bullet*>::iterator bullet;
		for (bullet = weapon->CurrentBullets()->begin(); bullet != weapon->CurrentBullets()->end(); ++bullet) {

			try
			{
				if (player.hasMoved)
				{
					(*bullet)->UpdatePosicion();
				}
				(*bullet)->render(mainShader);
			}
			catch (const std::exception &exc)
			{
				printf(exc.what());
			}
		}
		(*enemy)->renderEnemy(camera.Position, mainShader);
		//enemies.erase(std::remove(enemies.begin(), enemies.end(), *enemy), enemies.end());
	}
	
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
		std::vector<Enemy*>::iterator enemy;
		for (enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
			(*enemy)->fire(player.Position());
		}       
		player.Fire();
		break;
	}
	glutPostRedisplay();
}

void keyboardInput(unsigned char keycode, int x, int y) {
	    if (keycode == 'q') {
		    prueba = !prueba;
	    }
		if (prueba) {			
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
			case 32: // Escape key			
				player.hasJumped = true;
				break;			
			case 27: // Escape key			
				exit(EXIT_SUCCESS);
				break;
			}
			
		}
	
		glutPostRedisplay();						
}

float t = 0;
void onIdle()
{	
	t = t + 0.000002;
	if (player.hasMoved)
	{
		std::vector<Enemy*>::iterator enemy;
		for (enemy = enemies.begin(); enemy != enemies.end(); ++enemy) {
			(*enemy)->update(camera.Position);
		}
	}		

	std::vector<Bullet*>::iterator bullet;
	for (bullet = player.GetCurrentWeapon()->CurrentBullets()->begin(); bullet != player.GetCurrentWeapon()->CurrentBullets()->end(); ++bullet) {		
		try
		{
			if (player.hasMoved)
			{				
				(*bullet)->UpdatePosicion();
			}
		}
		catch (const std::exception &exc)
		{
			// catch anything thrown within try block that derives from std::exception
			printf(exc.what());
		}
	}

	if (player.GetCurrentWeapon()->HasFired())
	{
		if (player.GetCurrentWeapon()->GoingDown())
		{
			player.GetCurrentWeapon()->Recoil((player.GetCurrentWeapon()->Recoil() - 4.0f));
		}
		else
		{
			player.GetCurrentWeapon()->Recoil((player.GetCurrentWeapon()->Recoil() + 4.0f));
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

