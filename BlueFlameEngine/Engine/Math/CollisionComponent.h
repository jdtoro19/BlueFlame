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
		enum Collision_Type {
			BOX,
			SPHERE
		};

		CollisionComponent();
		~CollisionComponent();

		void setCollisionType(Collision_Type ct, std::vector<Vertex> vertexlist);
		void setCollisionType(Collision_Type ct, std::vector<ModelMesh> meshlist);
		
		Box getBoundingBox();

		void setPadding(glm::vec3 _padding);
		void Update(glm::vec3 pos, glm::vec3 _scale);
		void Render();

	private:
		Collision_Type *collisionType;
		Box *boundingBox;
		glm::vec3 padding;
	};
}

#endif
