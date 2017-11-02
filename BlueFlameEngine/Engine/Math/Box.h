#ifndef BOX_H
#define BOX_H

#include <glm\glm.hpp>

struct Box {

	// Values used when calculating collisions, and in inferring each other
	glm::vec3 centre;
	glm::vec3 scale;
	glm::vec3 min;
	glm::vec3 max;
	float l;
	float h;
	float w;

	// Sets centre based off of min and max axis values
	inline void SetCentreFromPoints() {
		centre = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	}

	// Sets dimensions based off of min and max axis values
	inline void SetDimensionsFromPoints() {
		l = max.x - min.x;
		h = max.y - min.y;
		w = max.z - min.z;
	}

	// Sets min and max axis values based off of centre, and dimensions
	inline void SetPointsFromCentre() {
		min.x = centre.x - (l * scale.x / 2);
		max.x = centre.x + (l * scale.x / 2);
		min.y = centre.y - (h * scale.y / 2);
		max.y = centre.y + (h * scale.y / 2);
		min.z = centre.z - (w * scale.z / 2);
		max.z = centre.z + (w * scale.z / 2);
	}

	// Setter used to give move the centre to another position
	inline void SetCentre(glm::vec3 pos) {
		centre = pos;
	}

	// Setter used to give a collision component scale based on world scale in the scene
	inline void SetScale(glm::vec3 _scale) {
		scale = _scale;
	}
};
#endif

