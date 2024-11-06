#include "Random.h"

#include <cmath>

float randomRange(const float minimum, const float maximum) {
    // Create random number in [minimum, maximum]
    float randomNumber = static_cast<float>(random()) / static_cast<float>(RAND_MAX);
    const float range = maximum - minimum;
    randomNumber *= range;
    return randomNumber + minimum;
}

Cartesian3 randomVector(const float minimum, const float maximum) {
    return {
        randomRange(minimum, maximum),
        randomRange(minimum, maximum),
        randomRange(minimum, maximum)
    };
}

Cartesian3 randomUnitVectorInUpwardsCone(const float minimumAngle,
                                         const float minimumLength,
                                         const float maximumLength) {
    // use the minimum vertical angle to compute a cosine *downward* from azimuth
    // i.e. use the sine instead
    const float minimumCosineValue = std::sin(minimumAngle);

    // loop until good vector
    while (true) {
        // random vector in a box
        Cartesian3 result = randomVector(-maximumLength, maximumLength);

        float resultLength = result.length();

        // discard vectors with bad lengths
        if (resultLength < minimumLength) {
            continue;
        }

        if (resultLength > maximumLength) {
            continue;
        }

        // take dot product and compare with minimum angle
        // if it's a satisfactory angle, return immediately
        if (result.dot(Cartesian3(0.0, 0.0, 1.0)) > minimumCosineValue * resultLength) {
            return result;
        }
    }
}
