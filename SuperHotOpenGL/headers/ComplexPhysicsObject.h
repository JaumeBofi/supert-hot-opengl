#pragma once

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <mesh.h>
#include <vector>

class ComplexPhysicsObject
{
public:	
	btCollisionShape* collision_shape;	
	btRigidBody* object;
	std::unique_ptr<btCollisionShape> _shape;
	btCollisionShape* CreateCollisionShape(std::vector<Mesh> meshes);	
};