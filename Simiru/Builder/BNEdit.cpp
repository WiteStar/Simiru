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
	QHBoxLayout* layout = StartPage(page);
	QWidget* widget = (QWidget*)layout->parent();

	line = new ElaLineEdit(widget);
	line->setEnabled(isEnabled);

	QObject::connect(enabler, &ElaToggleSwitch::toggled, [&](bool checked) { line->setEnabled(checked); });
	layout->addWidget(line);
	return true;
}

QString BNEdit::GetArg()
{
	return isEnabled ? arg + ' ' + line->text() : QString();
}