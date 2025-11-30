#include "stdafx.h"
#include "BNEdit.h"

#include "ElaText.h"
#include "ElaToggleSwitch.h"
#include "JsonUtils.h"

bool BNEdit::LoadJson(const QJsonObject& obj)
{
	if (BNBase::LoadJson(obj) == false)
		return false;

	PARSE_STRING(default_string);
	return true;
}

bool BNEdit::SetupPage(PageBase* page)
{
	QHBoxLayout* layout = StartPage(page);
	QWidget* widget = (QWidget*)layout->parent();

	line = new ElaLineEdit(widget);
	line->setEnabled(enabled_by_default);
	line->setText(default_string);

	QObject::connect(enabler, &ElaToggleSwitch::toggled, line, &ElaLineEdit::setEnabled);
	layout->addWidget(line);
	layout->addStretch();
	return true;
}

QString BNEdit::GetArg()
{
	return isEnabled ? arg + ' ' + line->text() : QString();
}