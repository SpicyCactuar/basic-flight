#include "SphereCollision.h"

#include <cmath>
#include <limits>

constexpr float FLOAT_EPS = std::numeric_limits<float>::epsilon();

bool isLessOrEqual(const float a, const float b) {
    // a <= b iff (a < b || a == b) iff (a < b || abs(a - b) < EPS)
    return a < b || std::abs(a - b) < FLOAT_EPS;
}

float distanceBetween(const Cartesian3& p, const Cartesian3& o) {
    // clang-format off
    return std::sqrt(std::pow(p.x - o.x, 2.0f) +
                     std::pow(p.y - o.y, 2.0f) +
                     std::pow(p.z - o.z, 2.0f));
    // clang-format on
}

/**
 * @brief perform a sphere-point collision
 *
 * @param center of the sphere
 * @param radius > 0 of the sphere
 * @param point to check against
 *
 * @return whether point is within the sphere = {center, radius}
 */
bool isSpherePointCollision(const Cartesian3& center, float radius, const Cartesian3& point) {
    return isLessOrEqual(distanceBetween(point, center), radius);
}

/**
 * @param center1 of the first sphere
 * @param radius1 > 0 of the first sphere
 * @param center2 of the second sphere
 * @param radius2 > 0 of the second sphere
 *
 * @return whether sphere1 and sphere2 are colliding
 */
bool isSphereSphereCollision(const Cartesian3& center1,
                             const float radius1,
                             const Cartesian3& center2,
                             const float radius2) {
    const float distanceBetweenCenters = distanceBetween(center1, center2);
    return isLessOrEqual(distanceBetweenCenters, radius1 + radius2);
}
