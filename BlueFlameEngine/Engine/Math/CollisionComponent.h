#ifndef COLLISIONCOMPONENT_H
#define COLLISIONCOMPONENT_H

#include "..\Core\Component.h"
//#include "..\Rendering\3D\ModelComponent.h"
//#include "..\Core\RenderComponent.h"
#include "..\Rendering\3D\Mesh.h"
#include "..\Rendering\3D\ModelMesh.h"
#include <iostream>
#include <vector>
#include "Box.h"

namespace ENGINE {

	class CollisionComponent : public Component {
	public:

		// Collision type defines the volume that will surround the object.
		// Box can be modified for more specific height, width, and length, 
		// but requires more calculations for collisions.

		// ===== SPHERE (WORK IN PROGRESS) ==========
		// Sphere can only have a centre and a radius,
		// but makes calculations for collisions easier.
		enum Collision_Type {
			BOX,
			SPHERE
		};

		//Default Constructor sets the padding to to nothing.
		CollisionComponent();
		~CollisionComponent();

		// Basic functions needed by every component.
		// Update is called every frame and requires position, scale to be set every frame when the object moves.
		// Also infers max and min value changes with regards to position and scale.
		void Update(glm::vec3 pos, glm::vec3 _scale);

		// Render is called every frame and does nothing for collisions.
		void Render();

		// Both functions create bounding volumes dependant on the collision type,
		// and creates max and min axis values based upon all given vertices of the object.
		// For objects with render components, and manually made shapes, it requires its list of vertices.
		// For objects with model components, with models and meshes, it requires its list of meshes.
		void CreateCollisionVolume(Collision_Type ct, std::vector<Vertex> vertexlist);
		void CreateCollisionVolume(Collision_Type ct, std::vector<ModelMesh> meshlist);

		// Getters for values used in collisions.
		Collision_Type GetCollisionType();
		Box GetBoundingBox();

		// Sets the padding of the bounding box, to be bigger or smaller than the list of vertices given.
		void SetBoxPadding(glm::vec3 _padding);

	private:

		// Values used in collisions.
		Collision_Type collisionType;
		Box *boundingBox;

		// Padding of the bounding box.
		glm::vec3 boxPadding;
	};
}

#endif
