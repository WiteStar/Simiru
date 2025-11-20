#include "stdafx.h"
#include "Translator.h"

Translator::Translator(QWidget* parent)
	: QTranslator(parent)
{
	QApplication::instance()->installTranslator(this);
}

bool Translator::AddTranslation(const QString& path)
{
	QStringList fileNames{ QLocale::system().name() + ".json","custom.json","en_US.json" };

	QDir dir(path);
	QString filePath;
	for (const QString& fileName : fileNames)
	{
		if (dir.exists(fileName))
		{
			filePath = dir.filePath(fileName);
			break;
		}
	}

	if (filePath.isEmpty())
	{
		qWarning() << path << " has no usable translation!";
		return false;
	}

	QFile file(path);
	if (file.open(QIODevice::ReadOnly) == false)
	{
		qWarning() << "Failed to open " << path << '!';
		return false;
	};

	QJsonParseError err;
	QJsonDocument json = QJsonDocument::fromJson(file.readAll(), &err);
	if (json.isEmpty() || json.isNull())
	{
		qWarning() << err.errorString();
		return false;
	}

	QJsonObject obj = json.object();
	for (const QString& key : obj.keys())
		translations[key] = obj[key].toString();

	return true;
}

void Translator::ClearTranslation()
{
	translations.clear();
}

QString Translator::translate(const char* context, const char* sourceText,
	const char* disambiguation, int n) const
{
	return translations[QString(sourceText)];
}