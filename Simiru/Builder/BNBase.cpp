#include "stdafx.h"
#include "BNBase.h"

#include "ElaScrollArea.h"
#include "ElaScrollBar.h"
#include "ElaText.h"
#include "JsonUtils.h"

bool BNBase::LoadJson(const QJsonObject& obj)
{
	PARSE_STRING(name);
	PARSE_STRING(arg);

	enabled_by_default = false;
	PARSE_BOOL(enabled_by_default);

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

	QHBoxLayout* layout = page->addGroup();
	QWidget* widget = (QWidget*)layout->parent();

	ElaScrollArea* scrollArea = new ElaScrollArea(widget);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setIsAnimation(Qt::Horizontal, true);
	scrollArea->setIsOverShoot(Qt::Horizontal, true);
	scrollArea->setMouseTracking(true);
	scrollArea->setWidgetResizable(1);
	scrollArea->setIsGrabGesture(1);

	ElaScrollBar* scrollBar = new ElaScrollBar(scrollArea->horizontalScrollBar(), scrollArea);
	scrollBar->setIsAnimation(true);

	QWidget* scrollWidget = new QWidget(scrollArea);
	scrollWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	scrollWidget->setStyleSheet("background-color:transparent;");
	scrollArea->setWidget(scrollWidget);

	ElaText* text = new ElaText(scrollWidget);
	text->setTextPixelSize(16);
	text->setText(name);

	QHBoxLayout* scrollLayout = new QHBoxLayout(scrollWidget);
	scrollLayout->addWidget(text);
	scrollLayout->addSpacing(10);

	enabler = new ElaToggleSwitch(widget);
	QObject::connect(enabler, &ElaToggleSwitch::toggled, [&](bool checked) { isEnabled = checked; });
	enabler->setIsToggled(isEnabled);

	layout->addWidget(scrollArea);
	layout->addWidget(enabler);
	layout->addSpacing(10);
	return scrollLayout;
}