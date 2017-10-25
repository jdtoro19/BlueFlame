#ifndef BOX_H
#define BOX_H

#include <glm\glm.hpp>

struct Box {
	glm::vec3 centre;
	glm::vec3 scale;
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;
	float l;
	float w;
	float h;

	inline void setCentreFromPoints() {
		centre = glm::vec3((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2);
	}

	inline void setDimensionsFromPoints() {
		l = maxX - minX;
		w = maxY - minY;
		h = maxZ - minZ;
	}

	inline void setPointsFromCentre() {
		minX = centre.x - (l * scale.x / 2);
		maxX = centre.x + (l * scale.x / 2);
		minY = centre.y - (w * scale.y / 2);
		maxY = centre.y + (w * scale.y / 2);
		minZ = centre.z - (h * scale.z / 2);
		maxZ = centre.z + (h * scale.z / 2);
	}

	inline void setCentre(glm::vec3 pos) {
		centre = pos;
	}

	inline void setScale(glm::vec3 _scale) {
		scale = _scale;
	}
};
#endif

