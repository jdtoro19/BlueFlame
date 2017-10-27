#include "CollisionComponent.h"

using namespace ENGINE;

#include <iostream>

CollisionComponent::CollisionComponent() {
	padding = glm::vec3(1.0f, 1.0f, 1.0f);
}

CollisionComponent::~CollisionComponent() {
	
}

void CollisionComponent::CreateCollisionVolume(Collision_Type ct, std::vector<ModelMesh> meshlist) {
	if (ct == SPHERE) {
		boundingBox = nullptr;
		collisionType = Collision_Type::SPHERE;
	}
	else if (ct == BOX) {
		boundingBox = new Box();
		collisionType = Collision_Type::BOX;
		boundingBox->minX = meshlist[0].vertices[0].Position.x;
		boundingBox->maxX = meshlist[0].vertices[0].Position.x;
							
		boundingBox->minY = meshlist[0].vertices[0].Position.y;
		boundingBox->maxY = meshlist[0].vertices[0].Position.y;
							
		boundingBox->minZ = meshlist[0].vertices[0].Position.z;
		boundingBox->maxZ = meshlist[0].vertices[0].Position.z;

		for (unsigned int i = 0; i < meshlist.size(); i++) {
			for (unsigned int j = 0; j < meshlist[i].vertices.size(); j++) {
				if (boundingBox->minX > meshlist[i].vertices[j].Position.x)
					boundingBox->minX = meshlist[i].vertices[j].Position.x;
										
				if (boundingBox->maxX < meshlist[i].vertices[j].Position.x)
					boundingBox->maxX = meshlist[i].vertices[j].Position.x;
										
				if (boundingBox->minY > meshlist[i].vertices[j].Position.y)
					boundingBox->minY = meshlist[i].vertices[j].Position.y;
										
				if (boundingBox->maxY < meshlist[i].vertices[j].Position.y)
					boundingBox->maxY = meshlist[i].vertices[j].Position.y;
										
				if (boundingBox->minZ > meshlist[i].vertices[j].Position.z)
					boundingBox->minZ = meshlist[i].vertices[j].Position.z;
										
				if (boundingBox->maxZ < meshlist[i].vertices[j].Position.z)
					boundingBox->maxZ = meshlist[i].vertices[j].Position.z;
			}
		}
		boundingBox->setCentreFromPoints();
		boundingBox->setDimensionsFromPoints();
	}
}

void CollisionComponent::CreateCollisionVolume(Collision_Type ct, std::vector<Vertex> vertexlist) {
	if (ct == SPHERE) {
		boundingBox = nullptr;
		collisionType = Collision_Type::SPHERE;
	}
	else if (ct == BOX) {
		boundingBox = new Box();
		collisionType = Collision_Type::BOX;
		boundingBox->minX = vertexlist[0].position.x;
		boundingBox->maxX = vertexlist[0].position.x;
							
		boundingBox->minY = vertexlist[0].position.y;
		boundingBox->maxY = vertexlist[0].position.y;
							
		boundingBox->minZ = vertexlist[0].position.z;
		boundingBox->maxZ = vertexlist[0].position.z;

		for (unsigned int i = 0; i < vertexlist.size(); i++) {
			if (boundingBox->minX > vertexlist[i].position.x)
				boundingBox->minX = vertexlist[i].position.x;
									
			if (boundingBox->maxX < vertexlist[i].position.x)
				boundingBox->maxX = vertexlist[i].position.x;
									
			if (boundingBox->minY > vertexlist[i].position.y)
				boundingBox->minY = vertexlist[i].position.y;
									
			if (boundingBox->maxY < vertexlist[i].position.y)
				boundingBox->maxY = vertexlist[i].position.y;
									
			if (boundingBox->minZ > vertexlist[i].position.z)
				boundingBox->minZ = vertexlist[i].position.z;
									
			if (boundingBox->maxZ < vertexlist[i].position.z)
				boundingBox->maxZ = vertexlist[i].position.z;
		}
		boundingBox->setCentreFromPoints();
		boundingBox->setDimensionsFromPoints();
	}
}

CollisionComponent::Collision_Type CollisionComponent::GetCollisionType() {
	return collisionType;
}

Box CollisionComponent::getBoundingBox() {
	return *boundingBox;
}

void CollisionComponent::setPadding(glm::vec3 _padding) {
	padding = _padding;
}

void CollisionComponent::Update(glm::vec3 pos, glm::vec3 _scale) {
	boundingBox->setCentre(pos);
	boundingBox->setScale(glm::vec3(_scale.x * padding.x, _scale.y * padding.y, _scale.z * padding.z));
	boundingBox->setPointsFromCentre();
}

void CollisionComponent::Render() {

}