#ifndef SPHERE_H
#define SPHERE_H

#include <glm\glm.hpp>

struct Sphere {
	glm::vec3 centre;
	float radius;
	glm::vec3 max;
	glm::vec3 min;
	float scale;

	inline void SetRadiusFromPoints() {
		radius = (glm::distance(min, max) / 2.0f) * scale;
	}

	inline void SetCentreFromPoints() {
		centre = (max + min) / 2.0f;
	}

	inline void SetPointsFromCentre() {
		max = glm::vec3(centre.x + radius, centre.y + radius, centre.z + radius) * scale;
		min = glm::vec3(centre.x - radius, centre.y - radius, centre.z - radius) * scale;
	}

	inline void SetCentre(glm::vec3 _centre) {
		centre = _centre;
	}

	inline void SetScale(float _scale) {
		scale = _scale;
	}
};


#endif