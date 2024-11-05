#ifndef SCENE
#define SCENE

#include "HomogeneousFaceSurface.h"
#include "Matrix4.h"
#include "Terrain.h"
#include "LavaBombParticle.h"
#include "Cartesian3.h"

// Measured in meters/frame
typedef unsigned int Speed;

constexpr float theta = 3.0f;
constexpr Speed minFlightSpeed = 0;
constexpr Speed maxFlightSpeed = 9;
constexpr Speed speedStep = 1;

const Cartesian3 forward(0.0, 1.0f, 0.0);

class Scene {
public:
    Terrain terrain;
    HomogeneousFaceSurface planeModel;
    HomogeneousFaceSurface lavaBombModel;

    // (x, y, z) |-> (x, z, -y)
    Matrix4 world2OpenGLMatrix;

    Scene(const Cartesian3& initialPosition);

    // timeStep is measured in meters/seconds to streamline calculations
    // Note: float allows for fractions of seconds
    void update(float timeStep);

    void render();

    // Rotate plane theta° around x-axis CW
    void pitchUp();

    // Rotate plane theta° around x-axis CCW
    void pitchDown();

    // Rotate plane theta° around y-axis CW
    void rollRight();

    // Rotate plane theta° around y-axis CCW
    void rollLeft();

    // Rotate plane theta° around z-axis CW
    void yawRight();

    // Rotate plane theta° around z-axis CCW
    void yawLeft();

    // Increases flight speed in forward direction by step, capped at maxFlightSpeed
    void increaseSpeed();

    // Increases flight speed in forward direction by step, capped at maxFlightSpeed
    void decreaseSpeed();

    bool shouldExit;

private:
    Cartesian3 planePosition;
    Matrix4 planeRotation;
    Speed flightSpeed;
    std::vector<LavaBombParticle> lavaBombs;
    // Measured in seconds, this allows to compute it as sum of timeSteps
    float chronometer;

    std::vector<Cartesian3> lavaBombCollisionPoints;

    // Returns C^(-1) derived from planePosition & planeRotation
    // C^(-1) = (T * R)^-1 = R^(-1) * T^(-1) = R^T * (-T)
    // R = cameraRotation
    // T = Matrix4::Translate(cameraPosition)
    Matrix4 inverseCameraMatrix;

    // Called on every Render()
    void updateCameraMatrix();

    Matrix4 computeViewMatrix(const Cartesian3& position) const;

    // Move plane in the forward direction times flightSpeed
    void movePlane();

    void updateLavaBombs(float timeStep);

    void refreshLavaBombs();

    void checkPlaneCollision();

    void checkLavaBombCollisions();

    // Must be called after updateCameraMatrix()
    void renderTerrain() const;

    void renderLavaBombs();
};

#endif
