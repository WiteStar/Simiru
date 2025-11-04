#include "stdafx.h"
#include "Simiru.h"
#include <QtWidgets/QApplication>

#include "ElaApplication.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    eApp->init();

    Simiru window;
    window.show();
    return app.exec();
}
