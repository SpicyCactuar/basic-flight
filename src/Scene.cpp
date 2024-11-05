#include "Scene.h"

#include <array>

#include "LavaBombParticle.h"
#include "Matrix4.h"
#include "Random.h"
#include "SphereCollision.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// three local variables with the hardcoded file names
const std::string groundModelName = "assets/landscape.dem";
const std::string planeModelName = "assets/planeModel.tri";
const std::string lavaBombModelName = "assets/lavaBombModel.tri";

const Homogeneous4 sunDirection(0.0, 0.3, 0.3, 1.0);
constexpr std::array<float, 4> groundColour = {0.2, 0.6, 0.2, 1.0};
constexpr std::array<float, 4> sunAmbient = {0.1, 0.1, 0.1, 1.0};
constexpr std::array<float, 4> sunDiffuse = {0.7, 0.7, 0.7, 1.0};
constexpr std::array<float, 4> blackColour = {0.0, 0.0, 0.0, 1.0};
constexpr std::array<float, 4> lavaBombColour = {0.5, 0.3, 0.0, 1.0};
constexpr std::array<float, 4> planeColour = {0.1, 0.1, 0.5, 1.0};
const Cartesian3 chaseCamVector(0.0, -2.0, 0.5);

const Cartesian3 worldOrigin(0.0f, 0.0f, 0.0f);
const Cartesian3 volcanoTip(-38500.0f, -4000.0f, 650.0f);

// Use maxFlightSpeed since it's the maximum translation in a single frame
constexpr float planeRadius = static_cast<float>(maxFlightSpeed);

constexpr int lavaBombsSpawnThreshold = 100;

constexpr float deltaTimeToSpawnParticle = 2.0f;
// An explosion triggers 6 Lava Bombs to be spawned from collision point
constexpr float explosionProbability = 0.3f;

Scene::Scene(const Cartesian3& initialPosition)
    : shouldExit(false),
      flightSpeed(0),
      chronometer(0.0f) {
    groundModel.readTerrainFile(groundModelName.data(), 500);
    planeModel.readTriangleSoupFile(planeModelName.data());
    lavaBombModel.readTriangleSoupFile(lavaBombModelName.data());

    /*
     * When modelling, z is commonly used for "vertical" with x-y used for "horizontal".
     * When rendering, the default is that we render using screen coordinates, so x is to the right,
     * y is up, and z points behind us by the right-hand rule. This means that when looking into the screen,
     * we are actually looking out along the z-axis.
     * That means we will have to start off with a view matrix that compensates for this.
     * Assume that we want to look out along the y-axis with the z-axis pointing up instead.
     * Then the x-axis is off to the right. This means that our mapping is as follows:
     *    x is unchanged   from WCS       to VCS
     *    y was forward    in WCS   is now up      in VCS
     *    z was up         in WCS   is now back    in VCS
     * Because x is unchanged, this is a rotation around x, with y moving towards z, so it is a
     * rotation of 90 degrees CCW.
     */
    world2OpenGLMatrix = Matrix4::rotationX(90.0);

    planeRotation = Matrix4::identity();

    planePosition = initialPosition;
}

void Scene::pitchUp() {
    planeRotation = Matrix4::rotationX(-theta) * planeRotation;
}

void Scene::pitchDown() {
    planeRotation = Matrix4::rotationX(theta) * planeRotation;
}

void Scene::rollLeft() {
    planeRotation = Matrix4::rotationY(-theta) * planeRotation;
}

void Scene::rollRight() {
    planeRotation = Matrix4::rotationY(theta) * planeRotation;
}

void Scene::yawLeft() {
    planeRotation = Matrix4::rotationZ(-theta) * planeRotation;
}

void Scene::yawRight() {
    planeRotation = Matrix4::rotationZ(theta) * planeRotation;
}

void Scene::increaseSpeed() {
    flightSpeed = flightSpeed < maxFlightSpeed ? flightSpeed + speedStep : maxFlightSpeed;
}

void Scene::decreaseSpeed() {
    flightSpeed = flightSpeed > minFlightSpeed ? flightSpeed - speedStep : minFlightSpeed;
}

void Scene::update(const float timeStep) {
    chronometer += timeStep;

    movePlane();
    updateLavaBombs(timeStep);
    checkPlaneCollision();
    checkLavaBombCollisions();
    refreshLavaBombs();
}

void Scene::movePlane() {
    if (flightSpeed > 0) {
        // Rotations don't affect length of vector
        // Since forward is a unit vector, || direction || = || planeRotation * forward || = 1
        // || translation || = || flightSpeed * direction || = flightSpeed * || direction || = flightSpeed
        // Therefore, the translation is always flightSpeed distance in forward direction
        const Cartesian3 translation = flightSpeed * (planeRotation * forward);
        planePosition = planePosition + translation;
    }
}

void Scene::updateLavaBombs(float timeStep) {
    for (auto& lavaBomb : lavaBombs) {
        if (lavaBomb.isAlive) {
            lavaBomb.update(timeStep);
        }
    }
}

void Scene::checkPlaneCollision() {
    // Check crash against terrain
    // This accounts for crashes from above or below the terrain
    const auto terrainPoint = Cartesian3(
        planePosition.x,
        planePosition.y,
        groundModel.getHeight(planePosition.x, planePosition.y));

    if (isSpherePointCollision(planePosition, planeRadius, terrainPoint)) {
        shouldExit = true;
    }

    // Check collision against each lava bomb
    for (const auto& lavaBomb : lavaBombs) {
        if (isSphereSphereCollision(planePosition, planeRadius, lavaBomb.position, lavaBombRadius)) {
            shouldExit = true;
        }
    }
}

void Scene::checkLavaBombCollisions() {
    for (unsigned int l1 = 0; l1 < lavaBombs.size(); l1++) {
        auto& lavaBomb = lavaBombs[l1];

        // Avoid unnecessary collisions
        // If more than 2 lava bombs are within collision range it will get detected
        if (!lavaBomb.isAlive) {
            continue;
        }

        for (unsigned int l2 = 0; l2 < lavaBombs.size(); l2++) {
            // Avoid self-collisions
            if (l1 != l2) {
                lavaBomb.checkCollision(lavaBombs[l2]);
            }
        }
    }
}

void Scene::refreshLavaBombs() {
    for (unsigned int i = 0; i < lavaBombs.size(); i++) {
        auto& lavaBomb = lavaBombs[i];

        if (!lavaBombs[i].isAlive) {
            lavaBombs.erase(lavaBombs.begin() + i);
            lavaBombCollisionPoints.emplace_back(lavaBomb.position);
        }
    }

    // Spawn 6 lava bombs from collision point of colliding lava bombs
    // With a probability of explosionProbability per collision point
    for (const auto& collisionPoint : lavaBombCollisionPoints) {
        if (const float roll = randomRange(0.0, 1.0f); roll > explosionProbability) {
            continue;
        }

        for (int i = 0; i < 6; i++) {
            lavaBombs.emplace_back(collisionPoint, &groundModel);
        }
    }
    lavaBombCollisionPoints.clear();

    // No need to do epsilon comparison, fine-grained accuracy is not needed
    if (chronometer >= deltaTimeToSpawnParticle && lavaBombs.size() < lavaBombsSpawnThreshold) {
        chronometer = 0.0f;
        lavaBombs.emplace_back(volcanoTip, &groundModel);
    }
}

void Scene::render() {
    // enable Z-buffering
    glEnable(GL_DEPTH_TEST);

    // set lighting parameters
    glShadeModel(GL_FLAT);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient.data());
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse.data());
    glLightfv(GL_LIGHT0, GL_SPECULAR, blackColour.data());
    glLightfv(GL_LIGHT0, GL_EMISSION, blackColour.data());

    // background is sky-blue
    glClearColor(0.7, 0.7, 1.0, 1.0);

    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // compute the light position
    // Translation matrices don't affect rotation component
    // (W2OGL * R^T) is the rotation component of the terrain viewMatrix
    Homogeneous4 lightDirection = world2OpenGLMatrix * planeRotation.transpose() * sunDirection;
    // and set the w to zero to force infinite distance
    lightDirection.w = 0.0;

    // pass it to OpenGL
    glLightfv(GL_LIGHT0, GL_POSITION, &lightDirection.x);

    // and set a material colour for the ground
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, groundColour.data());
    glMaterialfv(GL_FRONT, GL_SPECULAR, blackColour.data());
    glMaterialfv(GL_FRONT, GL_EMISSION, blackColour.data());

    updateCameraMatrix();

    renderTerrain();
    renderLavaBombs();
}

void Scene::updateCameraMatrix() {
    // C^(-1) = (T * R)^-1 = R^(-1) * T^(-1) = R^T * (-T)
    inverseCameraMatrix = planeRotation.transpose() * Matrix4::translation(-planePosition);
}

void Scene::renderTerrain() const {
    const Matrix4 terrainViewMatrix = computeViewMatrix(worldOrigin);
    groundModel.render(terrainViewMatrix);
}

void Scene::renderLavaBombs() {
    for (const auto& lavaBomb : lavaBombs) {
        Matrix4 lavaBombMatrix = computeViewMatrix(lavaBomb.position);
        lavaBombModel.render(lavaBombMatrix);
    }
}

Matrix4 Scene::computeViewMatrix(const Cartesian3& position) const {
    // 1. Translate the point in world coordinates to position
    // 2. Move and rotate the world inversely respect to the camera
    // 3. Apply W2OGL last to allow working with rendering axes
    return world2OpenGLMatrix * inverseCameraMatrix * Matrix4::translation(position);
}
