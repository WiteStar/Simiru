#include "stdafx.h"
#include "BuildInfo.h"
#include "JsonUtils.h"

BuildInfo::~BuildInfo()
{
	for (BNBase* node : nodes)
		delete node;
}

bool BuildInfo::ParseJson(const QJsonDocument& doc)
{
	qInfo() << "Parsing json";

	if (doc.isEmpty() || doc.isNull())
	{
		qWarning() << "Json is empty!";
		return false;
	}

	QJsonObject obj = doc.object();
	PARSE_STRING(executable_path);
	PARSE_STRING(translation_path);
	PARSE_STRING(default_args);

	PARSE_STRING(name);
	PARSE_STRING(introduction);

	if (auto e = obj["content"]; e.isNull())
		qWarning() << "content" << "is empty";
	else if (e.isArray() == false)
		qWarning() << "Wrong type for" << "content" << ": Array is expected, but type is" << e.type();
	else
	{
		QJsonArray array = e.toArray();

		int i = 0;
		for (QJsonValueRef ref : array)
		{
			qInfo() << "Parsing content[" << i++ << ']';
			if (ref.isObject() == false)
			{
				qWarning() << "Wrong type for" << "content[" << i << ']' << ": Object is expected, but type is" << e.type();
				continue;
			}
			QJsonObject obj = ref.toObject();

			QString type;
			PARSE_STRING(type);

			if (type.isEmpty())
				continue;

			BNBase* node;
			if (type == "text") node = new BNText();
			else if (type == "edit") node = new BNEdit();
			else if (type == "spin") node = new BNSpin();
			else if (type == "radio") node = new BNText();
			else if (type == "combo") node = new BNText();
			else if (type == "slider") node = new BNText();
			else if (type == "multicombo") node = new BNText();
			else { qWarning() << "Unknown type:" << type; continue; }

			if (node->LoadJson(obj))
				nodes.emplace_back(node);
			else
				delete node;
		}
	}
	qInfo() << "Finish parsing";
	return true;
}