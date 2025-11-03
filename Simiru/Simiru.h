#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Simiru.h"

class Simiru : public QMainWindow
{
    Q_OBJECT

public:
    Simiru(QWidget *parent = nullptr);
    ~Simiru();

private:
    Ui::SimiruClass ui;
};

