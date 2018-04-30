#ifndef VOLUMES_H
#define VOLUMES_H

#include <glm\glm.hpp>

struct Box {

	// Values used when calculating collisions, and in inferring each other
	glm::vec3 c;
	glm::vec3 r;

	// Sets centre based off of temp min and max axis values
	inline void SetCentreFromValues(glm::vec3 min, glm::vec3 max) {
		c = glm::vec3((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
	}

	// Sets half extents from the the temp min and max values
	inline void SetExtentsFromValues(glm::vec3 min, glm::vec3 max) {
		r.x = ((max.x - min.x) / 2.0f);
		r.y = ((max.y - min.y) / 2.0f);
		r.z = ((max.z - min.z) / 2.0f);
	}

	// Sets half extents based off of scale
	inline void SetExtentsFromScale(glm::vec3 scale) {
		r *= scale;
	}

	// Setter used to move the centre to another position
	inline void SetCentre(glm::vec3 pos) {
		c = pos;
	}

	// Assignment operator overload
	inline Box& operator=(const Box & other) {
		this->c = other.c;
		this->r = other.r;
	}
};

struct Sphere {
	glm::vec3 c;
	float r;

	// Sets centre based off of temp min and max axis values
	inline void SetCentreFromValues(glm::vec3 min, glm::vec3 max) {
		c = (min + max) / 2.0f;
	}

	// Sets radius from the the temp min and max values
	inline void SetRadiusFromValues(glm::vec3 min, glm::vec3 max) {
		r = glm::length(max - min) / 2.0f;
	}

	// Sets radius based off of scale
	inline void SetRadiusFromScale(glm::vec3 scale) {
		if (scale.x <= scale.y && scale.x <= scale.z) {
			r *= scale.x;
		}
		else if (scale.y <= scale.x && scale.y <= scale.z) {
			r *= scale.y;
		}
		else if (scale.z <= scale.z && scale.z <= scale.y) {
			r *= scale.z;
		}
	}

	// Setter used to move the centre to another position
	inline void SetCentre(glm::vec3 _centre) {
		c = _centre;
	}

	// Assignment operator overload
	inline Sphere& operator=(const Sphere & other) {
		this->c = other.c;
		this->r = other.r;
	}
};
#endif

