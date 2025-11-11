#pragma once

#include <QtWidgets/QMainWindow>

#include "ElaWindow.h"
#include "ElaContentDialog.h"

#include "Pages/PageBase.h"

class Simiru : public ElaWindow
{
	Q_OBJECT

public:
	Simiru(QWidget* parent = nullptr);
	~Simiru();

private:
	void initWindow();
	void initEdgeLayout();
	void initContent();

private:
	PageBase* base;
};