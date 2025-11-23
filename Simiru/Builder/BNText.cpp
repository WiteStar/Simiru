#include "stdafx.h"
#include "BNText.h"

#include "ElaText.h"
#include "ElaToggleSwitch.h"

bool BNText::LoadJson(const QJsonObject& obj)
{
	if (BNBase::LoadJson(obj) == false)
		return false;

	return true;
}

bool BNText::SetupPage(PageBase* page)
{
	if (BNBase::SetupPage(page) == false)
		return false;

	ElaText* text = new ElaText();
	text->setText(name);

	ElaToggleSwitch* enabler = new ElaToggleSwitch();
	QObject::connect(enabler, &ElaToggleSwitch::toggled, [&](bool checked) { isEnabled = checked; });
	enabler->setIsToggled(isEnabled);

	QHBoxLayout* layout = page->addGroup();
	layout->addWidget(text);
	layout->addStretch();
	layout->addWidget(enabler);
	layout->addSpacing(10);
	return true;
}

QString BNText::GetArg()
{
	return isEnabled ? arg : QString();
}