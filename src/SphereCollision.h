#ifndef SPHERE_COLLISION
#define SPHERE_COLLISION

#include "Cartesian3.h"

bool isSpherePointCollision(const Cartesian3& center, float radius, const Cartesian3& point);

bool isSphereSphereCollision(const Cartesian3& center1, float radius1, const Cartesian3& center2, float radius2);

#endif
