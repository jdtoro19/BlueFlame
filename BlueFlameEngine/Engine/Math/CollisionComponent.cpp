#include "CollisionComponent.h"

using namespace ENGINE;

#include <iostream>

CollisionComponent::CollisionComponent() {
	boxPadding = glm::vec3(1.0f, 1.0f, 1.0f);
}

CollisionComponent::~CollisionComponent() {
	boundingBox = nullptr;
	delete boundingBox;
}

void CollisionComponent::Update(glm::vec3 pos, glm::vec3 _scale) {
	boundingBox->SetCentre(pos);
	boundingBox->SetScale(glm::vec3(_scale.x * boxPadding.x, _scale.y * boxPadding.y, _scale.z * boxPadding.z));
	boundingBox->SetPointsFromCentre();
}

void CollisionComponent::Render() {

}

void CollisionComponent::CreateCollisionVolume(Collision_Type ct, std::vector<ModelMesh> meshlist) {
	// ===== SPHERE (WORK IN PROGRESS) ==========
	if (ct == SPHERE) {
		boundingBox = nullptr;
		collisionType = Collision_Type::SPHERE;
	}

	// Box min and max axis values being set
	else if (ct == BOX) {
		boundingBox = new Box();
		collisionType = Collision_Type::BOX;
		boundingBox->min.x = meshlist[0].vertices[0].Position.x;
		boundingBox->max.x = meshlist[0].vertices[0].Position.x;
							
		boundingBox->min.y = meshlist[0].vertices[0].Position.y;
		boundingBox->max.y = meshlist[0].vertices[0].Position.y;
							
		boundingBox->min.z = meshlist[0].vertices[0].Position.z;
		boundingBox->max.z = meshlist[0].vertices[0].Position.z;

		for (unsigned int i = 0; i < meshlist.size(); i++) {
			for (unsigned int j = 0; j < meshlist[i].vertices.size(); j++) {
				if (boundingBox->min.x > meshlist[i].vertices[j].Position.x)
					boundingBox->min.x = meshlist[i].vertices[j].Position.x;
										
				if (boundingBox->max.x < meshlist[i].vertices[j].Position.x)
					boundingBox->max.x = meshlist[i].vertices[j].Position.x;
										
				if (boundingBox->min.y > meshlist[i].vertices[j].Position.y)
					boundingBox->min.y = meshlist[i].vertices[j].Position.y;
										
				if (boundingBox->max.y < meshlist[i].vertices[j].Position.y)
					boundingBox->max.y = meshlist[i].vertices[j].Position.y;
										
				if (boundingBox->min.z > meshlist[i].vertices[j].Position.z)
					boundingBox->min.z = meshlist[i].vertices[j].Position.z;
										
				if (boundingBox->max.z < meshlist[i].vertices[j].Position.z)
					boundingBox->max.z = meshlist[i].vertices[j].Position.z;
			}
		}
		// Box centre and dimensions being set
		boundingBox->SetCentreFromPoints();
		boundingBox->SetDimensionsFromPoints();
	}
}

void CollisionComponent::CreateCollisionVolume(Collision_Type ct, std::vector<Vertex> vertexlist) {
	// ===== SPHERE (WORK IN PROGRESS) ==========
	if (ct == SPHERE) {
		boundingBox = nullptr;
		collisionType = Collision_Type::SPHERE;
	}
	// Box min and max axis values being set
	else if (ct == BOX) {
		boundingBox = new Box();
		collisionType = Collision_Type::BOX;
		boundingBox->min.x = vertexlist[0].position.x;
		boundingBox->max.x = vertexlist[0].position.x;
							
		boundingBox->min.y = vertexlist[0].position.y;
		boundingBox->max.y = vertexlist[0].position.y;
							
		boundingBox->min.z = vertexlist[0].position.z;
		boundingBox->max.z = vertexlist[0].position.z;

		for (unsigned int i = 0; i < vertexlist.size(); i++) {
			if (boundingBox->min.x > vertexlist[i].position.x)
				boundingBox->min.x = vertexlist[i].position.x;
									
			if (boundingBox->max.x < vertexlist[i].position.x)
				boundingBox->max.x = vertexlist[i].position.x;
									
			if (boundingBox->min.y > vertexlist[i].position.y)
				boundingBox->min.y = vertexlist[i].position.y;
									
			if (boundingBox->max.y < vertexlist[i].position.y)
				boundingBox->max.y = vertexlist[i].position.y;
									
			if (boundingBox->min.z > vertexlist[i].position.z)
				boundingBox->min.z = vertexlist[i].position.z;
									
			if (boundingBox->max.z < vertexlist[i].position.z)
				boundingBox->max.z = vertexlist[i].position.z;
		}
		// Box centre and dimensions being set
		boundingBox->SetCentreFromPoints();
		boundingBox->SetDimensionsFromPoints();
	}
}

CollisionComponent::Collision_Type CollisionComponent::GetCollisionType() {
	return collisionType;
}

Box CollisionComponent::GetBoundingBox() {
	return *boundingBox;
}

void CollisionComponent::SetBoxPadding(glm::vec3 _padding) {
	boxPadding = _padding;
}