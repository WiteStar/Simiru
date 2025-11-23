#pragma once
#include <QJsonObject>
#include "Pages/PageBase.h"

struct BNBase
{
	QString name;
	QVector<QString> suggestions;
	QString arg;
	bool isEnabled;

	virtual bool LoadJson(const QJsonObject& obj);
	virtual bool SetupPage(PageBase* page);
	virtual QString GetArg() = 0;
};