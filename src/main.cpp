#include <iostream>
#include <algorithm>
#include <QApplication>
#include "simulationwidget.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    SimulationWidget window;
    window.show();

    return app.exec();

}
