#pragma once

#include <QtWidgets/QMainWindow>

#include "ElaWindow.h"

class Simiru : public ElaWindow
{
    Q_OBJECT

public:
    Simiru(QWidget *parent = nullptr);
    ~Simiru();
};

