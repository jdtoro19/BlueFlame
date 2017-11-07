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
	if (collisionType == Collision_Type::SPHERE) {
		boundingSphere->SetCentre(pos);
		float scale;
		if (_scale.x <= _scale.y && _scale.x <= _scale.z) {
			scale = _scale.x;
		}
		else if (_scale.y <= _scale.x && _scale.y <= _scale.z) {
			scale = _scale.y;
		}
		else if (_scale.z <= _scale.x && _scale.z <= _scale.y) {
			scale = _scale.z;
		}
		boundingSphere->SetScale(scale * spherePadding);
		boundingSphere->SetPointsFromCentre();
	}
	else if (collisionType == Collision_Type::BOX) {
		boundingBox->SetCentre(pos);
		boundingBox->SetScale(glm::vec3(_scale.x * boxPadding.x, _scale.y * boxPadding.y, _scale.z * boxPadding.z));
		boundingBox->SetPointsFromCentre();
	}
}

void CollisionComponent::Render() {

}

void CollisionComponent::CreateCollisionVolume(Collision_Type ct, std::vector<ModelMesh> meshlist) {
	// Sphere min and max values being set
	if (ct == SPHERE) {
		boundingBox = nullptr;
		boundingSphere = new Sphere();
		collisionType = Collision_Type::SPHERE;

		boundingSphere->min.x = meshlist[0].vertices[0].Position.x;
		boundingSphere->max.x = meshlist[0].vertices[0].Position.x;
				
		boundingSphere->min.y = meshlist[0].vertices[0].Position.y;
		boundingSphere->max.y = meshlist[0].vertices[0].Position.y;
				
		boundingSphere->min.z = meshlist[0].vertices[0].Position.z;
		boundingSphere->max.z = meshlist[0].vertices[0].Position.z;

		for (unsigned int i = 0; i < meshlist.size(); i++) {
			for (unsigned int j = 0; j < meshlist[i].vertices.size(); j++) {
				if (boundingSphere->min.x > meshlist[i].vertices[j].Position.x)
					boundingSphere->min.x = meshlist[i].vertices[j].Position.x;
							
				if (boundingSphere->max.x < meshlist[i].vertices[j].Position.x)
					boundingSphere->max.x = meshlist[i].vertices[j].Position.x;
							
				if (boundingSphere->min.y > meshlist[i].vertices[j].Position.y)
					boundingSphere->min.y = meshlist[i].vertices[j].Position.y;
							
				if (boundingSphere->max.y < meshlist[i].vertices[j].Position.y)
					boundingSphere->max.y = meshlist[i].vertices[j].Position.y;
							
				if (boundingSphere->min.z > meshlist[i].vertices[j].Position.z)
					boundingSphere->min.z = meshlist[i].vertices[j].Position.z;
							
				if (boundingSphere->max.z < meshlist[i].vertices[j].Position.z)
					boundingSphere->max.z = meshlist[i].vertices[j].Position.z;
			}
		}
		// Sphere centre and radius being set
		boundingSphere->SetCentreFromPoints();
		boundingSphere->SetRadiusFromPoints();
	}

	// Box min and max axis values being set
	else if (ct == BOX) {
		boundingSphere = nullptr;
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
	// Sphere min and max values being set
	if (ct == SPHERE) {
		boundingBox = nullptr;
		boundingSphere = new Sphere();
		collisionType = Collision_Type::SPHERE;

		boundingSphere->min.x = vertexlist[0].position.x;
		boundingSphere->max.x = vertexlist[0].position.x;
				
		boundingSphere->min.y = vertexlist[0].position.y;
		boundingSphere->max.y = vertexlist[0].position.y;
				
		boundingSphere->min.z = vertexlist[0].position.z;
		boundingSphere->max.z = vertexlist[0].position.z;

		for (unsigned int i = 0; i < vertexlist.size(); i++) {
			if (boundingSphere->min.x > vertexlist[i].position.x)
				boundingSphere->min.x = vertexlist[i].position.x;
						
			if (boundingSphere->max.x < vertexlist[i].position.x)
				boundingSphere->max.x = vertexlist[i].position.x;
						
			if (boundingSphere->min.y > vertexlist[i].position.y)
				boundingSphere->min.y = vertexlist[i].position.y;
						
			if (boundingSphere->max.y < vertexlist[i].position.y)
				boundingSphere->max.y = vertexlist[i].position.y;
						
			if (boundingSphere->min.z > vertexlist[i].position.z)
				boundingSphere->min.z = vertexlist[i].position.z;
						
			if (boundingSphere->max.z < vertexlist[i].position.z)
				boundingSphere->max.z = vertexlist[i].position.z;
		}
		// Sphere centre and radius being set
		boundingSphere->SetCentreFromPoints();
		boundingSphere->SetRadiusFromPoints();
	}
	// Box min and max axis values being set
	else if (ct == BOX) {
		boundingSphere = nullptr;
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

Sphere CollisionComponent::GetBoundingSphere() {
	return *boundingSphere;
}

void CollisionComponent::SetBoxPadding(glm::vec3 _padding) {
	boxPadding = _padding;
}