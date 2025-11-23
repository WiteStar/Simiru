#pragma once
#include <QJsonObject>
#include "Pages/PageBase.h"

struct BNBase
{
	QString name;
	QVector<QString> suggestions;
	QString arg;

	virtual bool LoadJson(const QJsonObject& obj);
	virtual bool SetupPage(PageBase* page) = 0;
	virtual QString GetArg() = 0;
};