#pragma once

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>


class SimplePhysicsObject
{
private:
	bool dynamic;

	btConvexHullShape* unoptimized_hull;
	btShapeHull* hull_optimizer;
	btConvexHullShape* hull;

	btTriangleMesh* triangle_mesh;
	btBvhTriangleMeshShape* triangle_mesh_shape;

	btCollisionShape* collision_shape;
	btDefaultMotionState* motion_state;
	btScalar mass;
	btVector3 inertia;
	btRigidBody* object;
	
};
