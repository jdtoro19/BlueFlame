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
	float h;
	float w;

	inline void setCentreFromPoints() {
		centre = glm::vec3((minX + maxX) / 2, (minY + maxY) / 2, (minZ + maxZ) / 2);
	}

	inline void setDimensionsFromPoints() {
		l = maxX - minX;
		h = maxY - minY;
		w = maxZ - minZ;
	}

	inline void setPointsFromCentre() {
		minX = centre.x - (l * scale.x / 2);
		maxX = centre.x + (l * scale.x / 2);
		minY = centre.y - (h * scale.y / 2);
		maxY = centre.y + (h * scale.y / 2);
		minZ = centre.z - (w * scale.z / 2);
		maxZ = centre.z + (w * scale.z / 2);
	}

	inline void setCentre(glm::vec3 pos) {
		centre = pos;
	}

	inline void setScale(glm::vec3 _scale) {
		scale = _scale;
	}
};
#endif

