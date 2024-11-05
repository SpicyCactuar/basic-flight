#ifndef LAVA_BOMB_PARTICLE
#define LAVA_BOMB_PARTICLE

#include "Cartesian3.h"
#include "Terrain.h"

// Measured in meters/seconds
typedef float ParticleSpeed;

constexpr ParticleSpeed minParticleSpeed = 60.0f;
constexpr ParticleSpeed maxParticleSpeed = 300.0f;
constexpr float directionAngleRange = 45.0f;

constexpr ParticleSpeed gravity = 9.81f;

constexpr float lavaBombRadius = 100.0f;

class LavaBombParticle {
public:
    LavaBombParticle(const Cartesian3& initialPosition, const Terrain* terrain);

    Cartesian3 position;
    bool isAlive;

    void update(float timeStep);

    void checkCollision(LavaBombParticle& other);

private:
    Cartesian3 velocity;
    const Terrain* terrain;
    float lifespan;

    void checkTerrainCollision();
};

#endif
