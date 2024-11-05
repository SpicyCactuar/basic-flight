#include "FlightSimulatorWidget.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

constexpr float millisInFrame = 16.7f;
constexpr float millisInSecond = 1000.0f;
constexpr float timeStep = millisInFrame / millisInSecond;

FlightSimulatorWidget::FlightSimulatorWidget(QWidget* parent, Scene* scene)
    : _FLIGHT_SIMULATOR_PARENT_CLASS(parent),
      scene(scene) {
    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    animationTimer->start(millisInFrame);
}

void FlightSimulatorWidget::initializeGL() {
}

void FlightSimulatorWidget::resizeGL(const int width, const int height) {
    // reset the viewport
    glViewport(0, 0, width, height);

    // set projection matrix based on zoom & window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // compute the aspect ratio of the widget
    const float aspectRatio = static_cast<float>(width) / height;

    // we want a 90° vertical field of view, as wide as the window allows
    // and we want to see from just in front of us to 100km away
    gluPerspective(90.0, aspectRatio, 1, 100000);

    // set model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void FlightSimulatorWidget::paintGL() {
    scene->render();
}

void FlightSimulatorWidget::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_X:
            scene->shouldExit = true;
            break;
        case Qt::Key_A:
            scene->pitchDown();
            break;
        case Qt::Key_S:
            scene->pitchUp();
            break;
        case Qt::Key_Q:
            scene->rollLeft();
            break;
        case Qt::Key_E:
            scene->rollRight();
            break;
        case Qt::Key_W:
            scene->yawLeft();
            break;
        case Qt::Key_D:
            scene->yawRight();
            break;
        case Qt::Key_Plus:
            scene->increaseSpeed();
            break;
        case Qt::Key_Minus:
            scene->decreaseSpeed();
            break;
        default:
            break;
    }

    // always update to force the animation to continue
    _FLIGHT_SIMULATOR_UPDATE_CALL();
}

void FlightSimulatorWidget::nextFrame() {
    scene->update(timeStep);

    if (scene->shouldExit) {
        exit(0);
    }

    update();
}
