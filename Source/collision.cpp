#include "collision.h"
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <algorithm>

// Helper: clamp value between min and max
static float clampf(float v, float a, float b) {
    return std::max(a, std::min(b, v));
}

// Closest point on AABB to a point
static glm::vec3 closestPointOnAABB(const glm::vec3& pt, const glm::vec3& aabbMin, const glm::vec3& aabbMax) {
    return glm::vec3(
        clampf(pt.x, aabbMin.x, aabbMax.x),
        clampf(pt.y, aabbMin.y, aabbMax.y),
        clampf(pt.z, aabbMin.z, aabbMax.z)
    );
}

// Resolve collision between sphere and AABB by returning correction vector
static bool resolveSphereAABB(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& aabbMin, const glm::vec3& aabbMax, glm::vec3& outCorrection, float& outPenetrationDepth) {
    glm::vec3 closest = closestPointOnAABB(sphereCenter, aabbMin, aabbMax);
    glm::vec3 delta = sphereCenter - closest;
    float dist2 = glm::length2(delta);
    if (dist2 < 1e-8f) {
        // Sphere center is inside or exactly on surface; choose upward direction
        delta = glm::vec3(0.0f, 1.0f, 0.0f);
        dist2 = glm::length2(delta);
    }

    float dist = sqrt(dist2);
    float penetration = sphereRadius - dist;
    outPenetrationDepth = penetration;
    if (penetration > 0.0f) {
        outCorrection = (delta / (dist > 0.0f ? dist : 1.0f)) * penetration;
        return true;
    }
    outCorrection = glm::vec3(0.0f);
    return false;
}

void resolveCollisions(Camera& camera, const std::vector<GameObject>& objects, glm::vec3& outCorrection, bool& outGrounded, float capsuleHalfHeight, float capsuleRadius) {
    // Reset outputs
    outCorrection = glm::vec3(0.0f);
    outGrounded = false;

    // Capsule endpoints in world-space (vertical capsule along Y)
    glm::vec3 capsuleCenter = camera.Position;
    glm::vec3 top = capsuleCenter + glm::vec3(0.0f, capsuleHalfHeight, 0.0f);
    glm::vec3 bottom = capsuleCenter - glm::vec3(0.0f, capsuleHalfHeight, 0.0f);

    // We'll accumulate corrections and apply to camera.Position
    glm::vec3 totalCorrection(0.0f);
    float maxPenetrationDown = 0.0f; // track deepest penetration on bottom cap for grounded detection

    for (const auto& obj : objects) {
        if (obj.getType() != "cube") continue;

        glm::vec3 cubePos = obj.getPosition();
        glm::vec3 cubeSize = obj.getSize(); // assumed full size
        glm::vec3 scale = obj.getScale();

        // Apply scale to size
        glm::vec3 halfExtents = (cubeSize * scale) * 0.5f;

        // Compute AABB
        glm::vec3 aabbMin = cubePos - halfExtents;
        glm::vec3 aabbMax = cubePos + halfExtents;

        // 1) Check top sphere
        glm::vec3 corrTop(0.0f);
        float penTop = 0.0f;
        if (resolveSphereAABB(top, capsuleRadius, aabbMin, aabbMax, corrTop, penTop)) {
            totalCorrection += corrTop;
        }

        // 2) Check bottom sphere
        glm::vec3 corrBottom(0.0f);
        float penBottom = 0.0f;
        if (resolveSphereAABB(bottom, capsuleRadius, aabbMin, aabbMax, corrBottom, penBottom)) {
            totalCorrection += corrBottom;
            // If bottom cap was penetrating upward (i.e., collision pushed up), consider grounded
            if (corrBottom.y > 0.0f) {
                maxPenetrationDown = std::max(maxPenetrationDown, penBottom);
            }
        }

        // 3) Check cylindrical portion by sampling along segment
        const int samples = 4;
        for (int s = 0; s <= samples; ++s) {
            float t = (float)s / (float)samples;
            glm::vec3 samplePt = glm::mix(bottom, top, t);
            glm::vec3 corr(0.0f);
            float pen = 0.0f;
            if (resolveSphereAABB(samplePt, capsuleRadius, aabbMin, aabbMax, corr, pen)) {
                totalCorrection += corr * (1.0f / (samples + 1));
            }
        }
    }

    // Apply correction to camera position; cap max correction to avoid jitter
    if (glm::length2(totalCorrection) > 1e-8f) {
        // Prevent excessive correction per frame
        float maxCorrection = 1.0f; // 1 meter
        if (glm::length(totalCorrection) > maxCorrection) {
            totalCorrection = glm::normalize(totalCorrection) * maxCorrection;
        }
        outCorrection = totalCorrection;
    }

    // Grounded if bottom cap had a penetration depth greater than a small threshold
    outGrounded = (maxPenetrationDown > 0.001f);
}
