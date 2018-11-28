#pragma once

#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <weapon.h>
#include <worldObject.h>

class Player : public WorldObject
{
private:	
	vector<Weapon*> _weapons;
	glm::vec3 _position;
	int _currentWeapon;
	Camera* _currentCamera;

public:

	Player(glm::vec3 position,Camera* camera);	
	void AddWeapon(Weapon* weapon);
	Weapon* GetCurrentWeapon();
	glm::vec3 Position() const { return _position; }
	void Position(glm::vec3 val) { _position = val; }	

	void UpdatePosition();
	void Fire();
};