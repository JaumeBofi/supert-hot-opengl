#define _USE_MATH_DEFINES
#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <weapon.h>
#include <worldObject.h>
#include <player.h>
#include <math.h>



Player::Player(glm::vec3 position,Camera* camera)
{
	_weapons = vector<Weapon*>();
	_currentWeapon = -1;
	_currentCamera = camera;
}
void Player::AddWeapon(string modelName,int clipSize)
{
	Weapon* newWeapon = new Weapon("deagle.obj",clipSize);	
	_weapons.push_back(newWeapon);
	_currentWeapon = _weapons.size() - 1;
}

Weapon* Player::GetCurrentWeapon()
{
	return _weapons[_currentWeapon];
}

void Player::UpdatePosition()
{
	Position(_currentCamera->Position);

	glm::mat4 modelWeapon =
		glm::translate(glm::mat4(1.0f), _currentCamera->Position) * //llevar pistola junto a la posicion de la camara																				   		 		
		glm::rotate(glm::mat4(1.0f), glm::radians(-_currentCamera->Yaw-90.0f), _currentCamera->Up)*
		glm::rotate(glm::mat4(1.0f), glm::radians(_currentCamera->Pitch),_currentCamera->Right); //pistola sigue pitch de la camara
	printf("%f\n", _currentCamera->Yaw);
		
	Weapon* currentWeapon = GetCurrentWeapon();
	currentWeapon->UpdatePosition(modelWeapon);	
}