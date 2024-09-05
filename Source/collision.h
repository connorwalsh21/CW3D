#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>

// Axis-Aligned Bounding Box (AABB) structure
struct AABB {
    glm::vec3 min; // Minimum corner of the box
    glm::vec3 max; // Maximum corner of the box
    glm::vec3 velocity; // Velocity of the AABB
};

// Sphere structure
struct Sphere {
    glm::vec3 center; // Center of the sphere
    float radius;     // Radius of the sphere
    glm::vec3 velocity; // Velocity of the sphere
};

// Function prototypes
bool checkAABBCollision(const AABB& box1, const AABB& box2);
bool checkSphereCollision(const Sphere& sphere1, const Sphere& sphere2);
bool checkCubeSphereCollision(const AABB& box, const Sphere& sphere);
void handleCollisionAndStop(Sphere& sphere, const AABB& box);
void updateSphere(Sphere& sphere, float deltaTime);

#endif
