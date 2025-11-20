#pragma once
#include <QTranslator>
#include <QHash>
class Translator : public QTranslator
{
public:
	Translator(QWidget* parent = nullptr);

	bool AddTranslation(const QString& directory);
	void ClearTranslation();

	QString translate(const char* context, const char* sourceText,
		const char* disambiguation = nullptr, int n = -1) const override;
private:
	QHash<QString, QString> translations;
};