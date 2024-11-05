#ifndef RANDOM_H
#define RANDOM_H

#include "Cartesian3.h"

float randomRange(float minimum, float maximum);

Cartesian3 randomVector(float minimum, float maximum);

// generates a random vector with Monte Carlo simulation
Cartesian3 randomUnitVectorInUpwardsCone(float minimumAngle, float minimumLength, float maximumLength);

#endif
