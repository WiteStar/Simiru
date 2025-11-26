#include "stdafx.h"
#include "BNEdit.h"

#include "ElaText.h"
#include "ElaToggleSwitch.h"

bool BNEdit::LoadJson(const QJsonObject& obj)
{
	if (BNBase::LoadJson(obj) == false)
		return false;

	return true;
}

bool BNEdit::SetupPage(PageBase* page)
{
	if (BNBase::SetupPage(page) == false)
		return false;

	ElaText* text = new ElaText();
	text->setTextPixelSize(16);
	text->setText(name);

	line = new ElaLineEdit();
	line->setEnabled(isEnabled);

	ElaToggleSwitch* enabler = new ElaToggleSwitch();
	QObject::connect(enabler, &ElaToggleSwitch::toggled, [&](bool checked) { isEnabled = checked; line->setEnabled(checked); });
	enabler->setIsToggled(isEnabled);

	QHBoxLayout* layout = page->addGroup();
	layout->addWidget(text);
	layout->addSpacing(10);
	layout->addWidget(line);
	layout->addStretch();
	layout->addWidget(enabler);
	layout->addSpacing(10);
	return true;
}

QString BNEdit::GetArg()
{
	return isEnabled ? arg + ' ' + line->text() : QString();
}