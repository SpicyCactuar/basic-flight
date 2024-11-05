#include "LavaBombParticle.h"

#include "SphereCollision.h"
#include "Random.h"

// Measured in seconds, this allows to compute it as sum of timeSteps
constexpr float minimumLifespan = 3.0f;

LavaBombParticle::LavaBombParticle(const Cartesian3& initialPosition, const Terrain* terrain)
    : position(initialPosition),
      isAlive(true),
      terrain(terrain),
      lifespan(0.0f) {
    const float speed = randomRange(minParticleSpeed, maxParticleSpeed);
    const Cartesian3 direction = randomUnitVectorInUpwardsCone(directionAngleRange, 0.5f, 2.0f).unit();
    velocity = speed * direction;
}

void LavaBombParticle::update(float timeStep) {
    lifespan += timeStep;

    // Make gravity only affect the vertical axis
    velocity[2] = velocity[2] - gravity * timeStep;

    position = position + velocity * timeStep;

    checkTerrainCollision();
}

void LavaBombParticle::checkTerrainCollision() {
    /*
     * We check collision against terrain by seeing if the LavaBombParticle position projected
     * onto the terrain is within lava bomb's radius.
     *
     * The terrain point is (x, y, getHeight(x, y)), where (x, y) are taken from the lava bomb.
     * With that point, a point-sphere collision is detected by checking whether the
     * the terrain point is inside the sphere of radius lavaBombCollisionRadius.
     *
     * This logic purposefully bypasses the age check as it's not required.
     */
    if (const Cartesian3 terrainPoint(position.x, position.y, terrain->getHeight(position.x, position.y));
        isSpherePointCollision(this->position, lavaBombRadius, terrainPoint)) {
        isAlive = false;
    }
}

void LavaBombParticle::checkCollision(LavaBombParticle& other) {
    if (lifespan < minimumLifespan || other.lifespan < minimumLifespan) {
        return;
    }

    if (isSphereSphereCollision(
        this->position, lavaBombRadius,
        other.position, lavaBombRadius)) {
        isAlive = false;
        other.isAlive = false;
    }
}
