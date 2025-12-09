#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include "gameobject.hpp"
#include "camera.h"

// Resolve collisions between a vertical capsule (player) and cube colliders (GameObjects of type "cube").
// The capsule is centered on the camera position and aligned with the Y axis.
// capsuleHalfHeight is the half-distance from the capsule center to each sphere-cap end.
// capsuleRadius is the capsule radius.
// Outputs:
// - outCorrection: a translation vector that should be applied to the camera position to separate from colliders.
// - outGrounded: set to true if the capsule is considered on the ground after resolution (a collision pushed it up).
void resolveCollisions(Camera& camera, const std::vector<GameObject>& objects, glm::vec3& outCorrection, bool& outGrounded, float capsuleHalfHeight = 0.9f, float capsuleRadius = 0.5f);
