#ifndef INTERSECT_DATA_H
#define INTERSECT_DATA_H

#include "PhysicsComponent.h"
#include <glm\glm.hpp>

namespace ENGINE {

	struct IntersectData {
		PhysicsComponent *A;
		PhysicsComponent *B;
		glm::vec3 normal;
		float penatration;
		glm::vec3 mtv;
	};
}
#endif