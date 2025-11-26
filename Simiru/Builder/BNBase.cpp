#include "stdafx.h"
#include "BNBase.h"

#include "ElaText.h"
#include "JsonUtils.h"

bool BNBase::LoadJson(const QJsonObject& obj)
{
	PARSE_STRING(name);
	PARSE_STRING(arg);

	isEnabled = false;
	if (auto e = obj["enabled_by_default"]; e.isNull())
		qInfo() << "enabled_by_default" << "is empty";
	else if (e.isBool() == false)
		qWarning() << "Wrong type for" << "enabled_by_default" << ": Bool is expected, but type is" << e.type();
	else
		isEnabled = e.toBool();

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

QHBoxLayout* BNBase::StartPage(PageBase* page)
{
	ElaSuggestBox* suggestBox = page->getSuggestBox();
	for (const QString& suggestion : suggestions)
		suggestBox->addSuggestion(suggestion);

	ElaText* text = new ElaText();
	text->setTextPixelSize(16);
	text->setText(name);

	enabler = new ElaToggleSwitch();
	QObject::connect(enabler, &ElaToggleSwitch::toggled, [&](bool checked) { isEnabled = checked; });
	enabler->setIsToggled(isEnabled);

	QHBoxLayout* layout = page->addGroup();
	layout->addWidget(text);
	layout->addSpacing(10);
	return layout;
}

void BNBase::FinishPage(QHBoxLayout* layout)
{
	layout->addStretch();
	layout->addWidget(enabler);
	layout->addSpacing(10);
}
