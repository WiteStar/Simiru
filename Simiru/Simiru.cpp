#include "stdafx.h"
#include "Simiru.h"

Simiru::Simiru(QWidget* parent)
	: ElaWindow(parent)
{
	initWindow();
	initEdgeLayout();
	initContent();
}

Simiru::~Simiru()
{
}

void Simiru::initWindow()
{
	setUserInfoCardPixmap(QPixmap(":/Resource/Simiru.png"));
	setUserInfoCardTitle("Simiru");
	setUserInfoCardSubTitle("An exntendable toolbox");
}

void Simiru::initEdgeLayout()
{
}

void Simiru::initContent()
{
}