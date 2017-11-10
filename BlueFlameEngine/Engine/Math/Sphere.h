#ifndef SPHERE_H
#define SPHERE_H

#include <glm\glm.hpp>

struct Sphere {
	glm::vec3 centre;
	float radius;
	float diameter;
	glm::vec3 max;
	glm::vec3 min;
	float scale = 1.0f;

	inline void SetDiameterFromRadius() {
		diameter = radius * 2.0f;
	}

	inline void SetRadiusFromPoints() {
		radius = glm::length(max - centre) * scale;
	}

	inline void SetRadiusFromDiameter() {
		radius = (diameter / 2.0f) * scale;
	}

	inline void SetCentreFromPoints() {
		centre = (min + max) / 2.0f;
	}

	inline void SetPointsFromCentre() {
		max = glm::vec3(centre.x + radius, centre.y + radius, centre.z + radius);
		min = glm::vec3(centre.x - radius, centre.y - radius, centre.z - radius);
	}

	inline void SetCentre(glm::vec3 _centre) {
		centre = _centre;
	}

	inline void SetScale(float _scale) {
		scale = _scale;
	}
};


#endif