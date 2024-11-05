#ifndef FLIGHT_SIMULATOR_WIDGET
#define FLIGHT_SIMULATOR_WIDGET

#include <QtGlobal>
#include <QTimer>
#include <QMouseEvent>

// this is necessary to allow compilation in both Qt 5 and Qt 6
#if (QT_VERSION < 0x060000)
#include <QGLWidget>
#define _FLIGHT_SIMULATOR_PARENT_CLASS QGLWidget
#define _FLIGHT_SIMULATOR_UPDATE_CALL updateGL
#else
#include <QtOpenGLWidgets/QOpenGLWidget>
#define _GEOMETRIC_WIDGET_PARENT_CLASS QOpenGLWidget
#define _GL_WIDGET_UPDATE_CALL update
#endif

#include "Scene.h"

class FlightSimulatorWidget : public _FLIGHT_SIMULATOR_PARENT_CLASS {
    Q_OBJECT

public:
    Scene* scene;

    QTimer* animationTimer;

    FlightSimulatorWidget(QWidget* parent, Scene* scene);

protected:
    void initializeGL() override;

    void resizeGL(int width, int height) override;

    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;

public slots:
    void nextFrame();
};

#endif
