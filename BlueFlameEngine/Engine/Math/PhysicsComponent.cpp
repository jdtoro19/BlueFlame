#include "PhysicsComponent.h"

using namespace ENGINE;

PhysicsComponent::PhysicsComponent() {

}

PhysicsComponent::~PhysicsComponent() {

}

void PhysicsComponent::setColliderType(Collider_Type ct, ModelComponent &model) {
	if (ct == SPHERE) {

	}
	else if (ct == BOX) {
		Box boundingBox;

		boundingBox.minX = model.meshes[0].vertices[0].Position.x;
		boundingBox.maxX = model.meshes[0].vertices[0].Position.x;

		boundingBox.minY = model.meshes[0].vertices[0].Position.y;
		boundingBox.maxY = model.meshes[0].vertices[0].Position.y;

		boundingBox.minZ = model.meshes[0].vertices[0].Position.z;
		boundingBox.maxZ = model.meshes[0].vertices[0].Position.z;
		for (unsigned int i = 0; i < model.meshes.size(); i++) {
			for (unsigned int j = 0; j < model.meshes[i].vertices.size(); j++) {
				if (boundingBox.minX > model.meshes[i].vertices[j].Position.x)
					boundingBox.minX = model.meshes[i].vertices[j].Position.x;

				if (boundingBox.maxX < model.meshes[i].vertices[j].Position.x)
					boundingBox.maxX = model.meshes[i].vertices[j].Position.x;

				if (boundingBox.minY > model.meshes[i].vertices[j].Position.y)
					boundingBox.minY = model.meshes[i].vertices[j].Position.y;

				if (boundingBox.maxY < model.meshes[i].vertices[j].Position.y)
					boundingBox.maxY = model.meshes[i].vertices[j].Position.y;

				if (boundingBox.minZ > model.meshes[i].vertices[j].Position.z)
					boundingBox.minZ = model.meshes[i].vertices[j].Position.z;

				if (boundingBox.maxZ < model.meshes[i].vertices[j].Position.z)
					boundingBox.maxZ = model.meshes[i].vertices[j].Position.z;
			}
		}
	}
}

void PhysicsComponent::setColliderType(Collider_Type ct, RenderComponent &render) {
	Box boundingBox;

	boundingBox.minX = render.getVertexList()[0].position.x;
	boundingBox.maxX = render.getVertexList()[0].position.x;

	boundingBox.minY = render.getVertexList()[0].position.y;
	boundingBox.maxY = render.getVertexList()[0].position.y;

	boundingBox.minZ = render.getVertexList()[0].position.z;
	boundingBox.maxZ = render.getVertexList()[0].position.z;

	for (unsigned int i = 0; i < render.getVertexList().size(); i++) {
		if (boundingBox.minX > render.getVertexList()[i].position.x)
			boundingBox.minX = render.getVertexList()[i].position.x;

		if (boundingBox.maxX < render.getVertexList()[i].position.x)
			boundingBox.maxX = render.getVertexList()[i].position.x;

		if (boundingBox.minY > render.getVertexList()[i].position.y)
			boundingBox.minY = render.getVertexList()[i].position.y;

		if (boundingBox.maxY < render.getVertexList()[i].position.y)
			boundingBox.maxY = render.getVertexList()[i].position.y;

		if (boundingBox.minZ > render.getVertexList()[i].position.z)
			boundingBox.minZ = render.getVertexList()[i].position.z;

		if (boundingBox.maxZ < render.getVertexList()[i].position.z)
			boundingBox.maxZ = render.getVertexList()[i].position.z;
	}
}