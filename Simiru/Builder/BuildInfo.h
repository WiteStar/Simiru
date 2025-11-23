#pragma once
#include <QString>
#include <QVector>
#include "BuildNode.h"

struct BuildInfo
{
	QString executable_path;
	QString translation_path;
	QString default_args;

	QString name;
	QString introduction;
	QVector<BNBase*> nodes;

	~BuildInfo();
	bool ParseJson(const QJsonDocument& doc);
};