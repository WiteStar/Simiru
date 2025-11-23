#include "stdafx.h"
#include "BNBase.h"
#include "JsonUtils.h"

bool BNBase::LoadJson(const QJsonObject& obj)
{
	PARSE_STRING(name);
	PARSE_STRING(arg);

	auto e = obj["suggestions"];
	if (auto e = obj["suggestions"]; e.isNull())
	{
		qInfo() << "suggestions" << "is empty";
		suggestions.emplace_back(name);
	}
	else if (e.isArray() == false) \
		qWarning() << "Wrong type for" << "suggestions" << ": Array is expected, but type is " << e.type();
	else
	{
		int i = 0;
		QJsonArray array = e.toArray();
		for (QJsonValueRef ref : array)
		{
			i++;
			if (ref.isString() == false)
			{
				qWarning() << "Wrong type for" << "suggestions[" << i << ']' << ": String is expected, but type is " << e.type();
				continue;
			}
			suggestions.emplace_back(ref.toString());
		}
	}
	return true;
}