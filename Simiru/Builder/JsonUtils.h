#pragma once

#define PARSE_STRING(variable) \
if (auto e = obj[#variable]; e.isNull()) \
	qInfo() << #variable << "is empty"; \
else if (e.isString() == false) \
	qWarning() << "Wrong type for" << #variable <<": String is expected, but type is" << e.type(); \
else \
	variable = e.toString(); \

#define PARSE_NUMBER(variable) \
if (auto e = obj[#variable]; e.isNull()) \
	qInfo() << #variable << "is empty"; \
else if (e.isDouble() == false) \
	qWarning() << "Wrong type for" << #variable <<": Double(Integer) is expected, but type is" << e.type(); \
else \
	variable = e.toDouble(); \

#define PARSE_BOOL(variable) \
if (auto e = obj[#variable]; e.isNull()) \
	qInfo() << #variable << "is empty"; \
else if (e.isBool() == false) \
	qWarning() << "Wrong type for" << #variable << ": Bool is expected, but type is" << e.type(); \
else \
	variable = e.toBool(); \
