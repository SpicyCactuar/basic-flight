#include <QtWidgets/QApplication>
#include <cstdlib>
#include <iostream>
#include <string>

#include "Cartesian3.h"
#include "FlightSimulatorWidget.h"
#include "Scene.h"

int main(int argc, char** argv) {
    QApplication application(argc, argv);

    if (argc != 4) {
        std::cerr << "Application should receive 3 parameters specifying initial (x, y, z) coordinates" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        const Cartesian3 initialPosition(atof(argv[1]), atof(argv[2]), atof(argv[3]));
        Scene scene(initialPosition);

        FlightSimulatorWidget flightWindow(nullptr, &scene);
        flightWindow.resize(1200, 675);
        flightWindow.show();

        return application.exec();
    } catch (std::string errorString) {
        std::cout << "Unable to run application." << errorString << std::endl;
        return EXIT_FAILURE;
    }
}
