#include "Collision.h"
#include <glm/gtx/norm.hpp> // Include for distance calculations
#include <iostream>

// Check collision between two AABBs
bool checkAABBCollision(const AABB& box1, const AABB& box2) {
    return (box1.min.x <= box2.max.x && box1.max.x >= box2.min.x) &&
        (box1.min.y <= box2.max.y && box1.max.y >= box2.min.y) &&
        (box1.min.z <= box2.max.z && box1.max.z >= box2.min.z);
}

// Check collision between two spheres
bool checkSphereCollision(const Sphere& sphere1, const Sphere& sphere2) {
    float distance = glm::distance(sphere1.center, sphere2.center);
    return distance < (sphere1.radius + sphere2.radius);
}

// Check collision between a cube and a sphere
bool checkCubeSphereCollision(const AABB& box, const Sphere& sphere) {
    glm::vec3 closestPoint = glm::clamp(sphere.center, box.min, box.max);
    float distanceSquared = glm::distance2(closestPoint, sphere.center);
    return distanceSquared <= (sphere.radius * sphere.radius);
}

// Function to stop the sphere's movement on collision with a cube
void handleCollisionAndStop(Sphere& sphere, const AABB& box) {
    if (checkCubeSphereCollision(box, sphere)) {
        // Stop the sphere by setting its velocity to zero
        sphere.velocity = glm::vec3(0.0f);

        // Optional: Correct the sphere's position slightly to prevent sticking
        glm::vec3 correction = glm::normalize(sphere.center - glm::clamp(sphere.center, box.min, box.max)) * 0.01f;
        sphere.center += correction;
    }
}

// Update the position of the sphere based on its velocity
void updateSphere(Sphere& sphere, float deltaTime) {
    // Update the sphere's position
    sphere.center += sphere.velocity * deltaTime;
}
