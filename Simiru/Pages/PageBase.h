#pragma once

#include "ElaScrollPage.h"

class PageBase : public ElaScrollPage
{
	Q_OBJECT

public:
	PageBase(const QString& title, const QString& subtitle, QWidget* parent = nullptr);
	PageBase(QWidget* parent = nullptr);
	~PageBase();
};