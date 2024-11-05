QT+=opengl
LIBS+=-lGLU
TEMPLATE = app
TARGET = ./bin/basic-flight
INCLUDEPATH += ./src
OBJECTS_DIR=./build/obj
MOC_DIR=./build/moc

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/Cartesian3.h \
           src/FlightSimulatorWidget.h \
           src/Homogeneous4.h \
           src/HomogeneousFaceSurface.h \
           src/LavaBombParticle.h \
           src/Matrix4.h \
           src/Random.h \
           src/Scene.h \
           src/SphereCollision.h \
           src/Terrain.h

SOURCES += src/Cartesian3.cpp \
           src/FlightSimulatorWidget.cpp \
           src/Homogeneous4.cpp \
           src/HomogeneousFaceSurface.cpp \
           src/LavaBombParticle.cpp \
           src/main.cpp \
           src/Matrix4.cpp \
           src/Random.cpp \
           src/Scene.cpp \
           src/SphereCollision.cpp \
           src/Terrain.cpp
