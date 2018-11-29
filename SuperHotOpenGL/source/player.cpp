#define _USE_MATH_DEFINES
#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <weapon.h>
#include <worldObject.h>
#include <player.h>
#include <math.h>
#define EPSILON 0.001


Player::Player(glm::vec3 position,Camera* camera)
{
	_weapons = vector<Weapon*>();
	_currentWeapon = -1;
	_currentCamera = camera;
}
void Player::AddWeapon(Weapon* weapon)
{
	Weapon* newWeapon = weapon;
	_weapons.push_back(newWeapon);
	_currentWeapon = _weapons.size() - 1;
}

Weapon* Player::GetCurrentWeapon()
{
	return _weapons[_currentWeapon];
}

void Player::UpdatePosition(Model* scene)
{
	glm::vec3 v = Position() - _currentCamera->Position;
	hasMoved = !(glm::all(glm::lessThan(glm::abs(v), glm::vec3(EPSILON))));	
	hasMoved = Position() != _currentCamera->Position;	
	Position(_currentCamera->Position);
	Weapon* currentWeapon = GetCurrentWeapon();

	glm::mat4 modelWeapon =
		glm::translate(glm::mat4(1.0f), _currentCamera->Position) * //llevar pistola junto a la posicion de la camara																				   		 				
		glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), _currentCamera->Up)*
		glm::rotate(glm::mat4(1.0f), glm::radians(currentWeapon->Recoil()), _currentCamera->Front)*
		glm::rotate(glm::mat4(1.0f), glm::radians(-_currentCamera->Yaw), _currentCamera->Up)*		
		glm::rotate(glm::mat4(1.0f), glm::radians(_currentCamera->Pitch), _currentCamera->Right);
								
	currentWeapon->UpdatePosition(scene,modelWeapon);	
}

void Player::Fire()
{
	GetCurrentWeapon()->Fire(_position, _currentCamera->Front);
	// Ver las si tengo que recargar
}