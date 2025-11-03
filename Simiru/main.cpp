#include "stdafx.h"
#include "Simiru.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Simiru window;
    window.show();
    return app.exec();
}
