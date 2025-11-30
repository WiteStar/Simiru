#pragma once
#include <QJsonObject>
#include "ElaToggleSwitch.h"
#include "Pages/PageBase.h"

class BNBase
{
public:
	virtual bool LoadJson(const QJsonObject& obj);
	virtual bool SetupPage(PageBase* page) = 0;
	virtual QString GetArg() = 0;

protected:
	QHBoxLayout* StartPage(PageBase* page);

protected:
	QString name;
	QVector<QString> suggestions;
	QString arg;

	ElaToggleSwitch* enabler;
	bool isEnabled;
};