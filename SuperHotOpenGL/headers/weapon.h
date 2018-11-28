#pragma once

#include <model.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <bullet.h>
#include <vector>
#include <string.h>
#include <worldObject.h>
#include <camera.h>
#include <model.h>

using namespace std;

class Weapon : public  WorldObject
{
private:	
	int _clipSize;
	float _recoil = 0.0f;
	bool _hasFired = false;
	bool _goingDown = false;
	vector<Bullet*> _currentBullets;
	vector<Bullet*> _shotBullets;
	Model* _bulletMesh;

public:
	Weapon(string modelName,string bulletModelname,int clipSize);

	float Recoil() const { return _recoil; }
	void Recoil(float val) 
	{ 
		_recoil = val; 
		if (_recoil <= 0.0f)
		{
			_hasFired = false;
			_goingDown = false;
		}		

		if (_recoil >= 12.0f) _goingDown = true;
	}

	float GoingDown() const { return _goingDown; }
	float HasFired() const { return _hasFired; }
	
	vector<Bullet*>* CurrentBullets() { return &_currentBullets; }

	void UpdatePosition(glm::mat4 model);
	Bullet* Fire(glm::vec3 initialPosition,glm::vec3 direction);
	Bullet* Weapon::Fire(glm::vec3 initialPosition, glm::vec3 direction, float velocity);
};