#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <ComplexPhysicsObject.h>
#include <mesh.h>
#include <vector>
#include <cstddef>



btCollisionShape* ComplexPhysicsObject::CreateCollisionShape(vector<Mesh> meshes)
{		
	std::unique_ptr<btTriangleIndexVertexArray> _shape_indexed_vertex_array = std::make_unique<btTriangleIndexVertexArray>();

	for (auto& mesh : meshes)
	{
		btIndexedMesh indexed_mesh;

		indexed_mesh.m_numTriangles = mesh.indices.size() / 3;
		indexed_mesh.m_triangleIndexBase = (const unsigned char *)mesh.indices[0];
		indexed_mesh.m_triangleIndexStride = 3 * sizeof(GLint);
		indexed_mesh.m_indexType = PHY_INTEGER;

		indexed_mesh.m_numVertices = mesh.vertices.size();
		indexed_mesh.m_vertexBase = (const unsigned char *)(&mesh.vertices[0] + offsetof(Vertex, Position));
		indexed_mesh.m_vertexStride = sizeof(Vertex);
		indexed_mesh.m_vertexType = PHY_FLOAT; //set that we are dealing with floats

		_shape_indexed_vertex_array->addIndexedMesh(indexed_mesh);
	}
	//TODO Convex Decompositions.
	_shape = std::make_unique<btConvexTriangleMeshShape>(_shape_indexed_vertex_array.get());
	return _shape.get();
}